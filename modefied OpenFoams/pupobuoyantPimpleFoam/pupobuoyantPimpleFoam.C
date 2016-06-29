/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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
#include "fvIOoptionList.H"
#include "pimpleControl.H"
#include "fixedFluxPressureFvPatchScalarField.H"
#include "specie.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    pimpleControl pimple(mesh);

    #include "createFields.H"
    #include "createMRF.H"
    #include "createFvOptions.H"
    #include "createRadiationModel.H"
    #include "initContinuityErrs.H"
    #include "createTimeControls.H"
    #include "compressibleCourantNo.H"
    #include "setInitialDeltaT.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    label inlet     = mesh.boundaryMesh().findPatchID("Inlet"); 
    label outlet    = mesh.boundaryMesh().findPatchID("Outlet");

    while (runTime.run())
    {
        #include "createTimeControls.H"
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

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s" << "  ClockTime = " << runTime.elapsedClockTime() << " s"  << endl;
        Info<< "Inflow      : " << -1.0* sum(phi.boundaryField()[inlet])   <<" [kg/s]" << endl;
        Info<< "Outflow     : " <<       sum(phi.boundaryField()[outlet])  <<" [kg/s]" <<  endl;
        Info<< "EnergyInflow  : " << -1.0* sum( phi.boundaryField()[inlet]  * ( thermo.he().boundaryField()[inlet]  + 0.5*magSqr(U.boundaryField()[inlet])  ) )   <<" [W]"  <<  endl;
        Info<< "EnergyOutflow : " <<       sum( phi.boundaryField()[outlet] * ( thermo.he().boundaryField()[outlet] + 0.5*magSqr(U.boundaryField()[outlet]) ) )   <<" [W]"  <<  endl;   

        Info<< "rho max/avg/min : " << max(thermo.rho()).value() << " " << average(thermo.rho()).value() << " " << min(thermo.rho()).value() << endl;
        Info<< "T   max/avg/min : " << max(thermo.T()).value()   << " " << average(thermo.T()).value()   << " " << min(thermo.T()).value()   << endl;
        Info<< "P   max/avg/min : " << max(thermo.p()).value()   << " " << average(thermo.p()).value()   << " " << min(thermo.p()).value()   << endl;
        Info<< "Prg max/avg/min : " << max(p_rgh).value()        << " " << average(p_rgh).value()        << " " << min(p_rgh).value()        << endl;
        Info<< " " << endl;

    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
