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
    buoyantSimpleFoam

Description
    Steady-state solver for buoyant, turbulent flow of compressible fluids,
    including radiation, for ventilation and heat-transfer.

\*---------------------------------------------------------------------------*/
#include "fvCFD.H"
#include "rhoThermo.H"
#include "turbulentFluidThermoModel.H"
#include "radiationModel.H"
#include "simpleControl.H"
#include "fvOptions.H"
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

    turbulence->validate();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting Iteration loop\n" << endl;
    
    label inlet  = mesh.boundaryMesh().findPatchID("Inlet"); 
    label outlet = mesh.boundaryMesh().findPatchID("Outlet");

    while (simple.loop())
    {
        Info<< "Iteration Number = " << runTime.timeName() << nl << endl;

        // Pressure-velocity SIMPLE corrector
        {
            #include "UEqn.H"
            #include "EEqn.H"
            #include "pEqn.H"
        }
        
        turbulence->correct();

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

    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
