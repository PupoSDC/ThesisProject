/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    buoyantPimpleFoam

Description
    Transient solver for buoyant, turbulent flow of compressible fluids for
    ventilation and heat-transfer.

    Turbulence is modelled using a run-time selectable compressible RAS or
    LES model.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "rhoThermo.H"
#include "turbulentFluidThermoModel.H"
#include "radiationModel.H"
#include "fvOptions.H"
#include "pimpleControl.H"
#include "specie.H"
    
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "postProcess.H"

    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createFields.H"
    #include "createFvOptions.H"
    #include "initContinuityErrs.H"
    #include "createTimeControls.H"
    #include "compressibleCourantNo.H"
    #include "setInitialDeltaT.H"

    turbulence->validate();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "compressibleCourantNo.H"
        #include "setDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        #include "rhoEqn.H"

        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "UEqn.H"
            #include "EEqn.H"

            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.H"
            }

            if (pimple.turbCorr())
            {
                turbulence->correct();
            }
        }

        rho = thermo.rho();

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"  << "  ClockTime = " << runTime.elapsedClockTime() << " s"  << endl;

        Info<< "Inflow      : "   << -1.0* sum(phi.boundaryField()[inlet])   <<" [kg/s]" << endl;
        Info<< "Outflow     : "   <<       sum(phi.boundaryField()[outlet])  <<" [kg/s]" <<  endl;

        Info<< "EnergyInflow  : " << -1.0* sum( phi.boundaryField()[inlet]  * ( thermo.he().boundaryField()[inlet]  + 0.5*magSqr(U.boundaryField()[inlet])  ) )   <<" [W]"  <<  endl;
        Info<< "EnergyOutflow : " <<       sum( phi.boundaryField()[outlet] * ( thermo.he().boundaryField()[outlet] + 0.5*magSqr(U.boundaryField()[outlet]) ) )   <<" [W]"  <<  endl;   
        Info<< "EnergyBalance : " <<       sum( phi.boundaryField()[outlet] * ( thermo.he().boundaryField()[outlet] + 0.5*magSqr(U.boundaryField()[outlet]) ) ) 
                                     +1.0* sum( phi.boundaryField()[inlet]  * ( thermo.he().boundaryField()[inlet]  + 0.5*magSqr(U.boundaryField()[inlet])  ) )   <<" [W]"  <<  endl;

        Info<< "rho max/avg/min : " << gMax(thermo.rho()) << " " << gAverage(thermo.rho()) << " " << gMin(thermo.rho()) << endl;
        Info<< "T   max/avg/min : " << gMax(thermo.T())   << " " << gAverage(thermo.T())   << " " << gMin(thermo.T())   << endl;
        Info<< "P   max/avg/min : " << gMax(thermo.p())   << " " << gAverage(thermo.p())   << " " << gMin(thermo.p())   << endl;
        Info<< "Prg max/avg/min : " << gMax(p_rgh)        << " " << gAverage(p_rgh)        << " " << gMin(p_rgh)        << endl;
        Info<< "U   max/avg/min : " << max(U.component(2)).value() << " " <<  average(U.component(2)).value() << " " <<  min(U.component(2)).value() << endl;
        Info<< "Prg max-min : " << gMax(p_rgh)   -  gMin(p_rgh)        << endl;
        Info<< " " << endl;
        Info<< "sngrad(rho) :" << gMax(snGradRho.boundaryField()[outlet]) << " " << gAverage(snGradRho.boundaryField()[outlet]) << " " << gMin(snGradRho.boundaryField()[outlet]) << endl;
        Info<< " " << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
