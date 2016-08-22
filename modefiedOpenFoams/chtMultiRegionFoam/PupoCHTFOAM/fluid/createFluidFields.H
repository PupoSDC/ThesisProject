// Initialise fluid field pointer lists
PtrList<rhoThermo> thermoFluid(fluidRegions.size());
PtrList<volScalarField> rhoFluid(fluidRegions.size());
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
PtrList<uniformDimensionedVectorField> gFluid(fluidRegions.size());
PtrList<uniformDimensionedScalarField> hRefFluid(fluidRegions.size());
PtrList<volScalarField> ghFluid(fluidRegions.size());
PtrList<surfaceScalarField> ghfFluid(fluidRegions.size());
PtrList<compressible::turbulenceModel> turbulence(fluidRegions.size());
PtrList<volScalarField> p_rghFluid(fluidRegions.size());
PtrList<radiation::radiationModel> radiation(fluidRegions.size());

List<scalar> initialMassFluid(fluidRegions.size());
List<label> pRefCellFluid(fluidRegions.size(), 0);
List<scalar> pRefValueFluid(fluidRegions.size(), 0.0);
List<bool> frozenFlowFluid(fluidRegions.size(), false);

PtrList<dimensionedScalar> rhoMax(fluidRegions.size());
PtrList<dimensionedScalar> rhoMin(fluidRegions.size());

PtrList<IOMRFZoneList> MRFfluid(fluidRegions.size());
PtrList<fv::IOoptionList> fluidFvOptions(fluidRegions.size());

// Populate fluid field pointer lists
forAll(fluidRegions, i)
{
    Info<< "*** Reading fluid mesh thermophysical properties for region "
        << fluidRegions[i].name() << nl << endl;

    Info<< "    Adding to thermoFluid\n" << endl;

    thermoFluid.set
    (
        i,
        rhoThermo::New(fluidRegions[i]).ptr()
    );

    Info<< "    Adding to rhoFluid\n" << endl;
    rhoFluid.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "rho",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            thermoFluid[i].rho()
        )
    );

    Info<< "    Adding to UFluid\n" << endl;
    UFluid.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "U",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );

    Info<< "    Adding to phiFluid\n" << endl;
    phiFluid.set
    (
        i,
        new surfaceScalarField
        (
            IOobject
            (
                "phi",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            linearInterpolate(rhoFluid[i]*UFluid[i])
                & fluidRegions[i].Sf()
        )
    );

    Info<< "    Adding to gFluid\n" << endl;
    gFluid.set
    (
        i,
        new uniformDimensionedVectorField
        (
            IOobject
            (
                "g",
                runTime.constant(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        )
    );

    Info<< "    Adding to hRefFluid\n" << endl;
    hRefFluid.set
    (
        i,
        new uniformDimensionedScalarField
        (
            IOobject
            (
                "hRef",
                runTime.constant(),
                fluidRegions[i],
                IOobject::READ_IF_PRESENT,
                IOobject::NO_WRITE
            ),
            dimensionedScalar("hRef", dimLength, 0)
        )
    );

    dimensionedScalar ghRef
    (
        mag(gFluid[i].value()) > SMALL
      ? gFluid[i]
          & (cmptMag(gFluid[i].value())/mag(gFluid[i].value()))*hRefFluid[i]
      : dimensionedScalar("ghRef", gFluid[i].dimensions()*dimLength, 0)
    );

    Info<< "    Adding to ghFluid\n" << endl;
    ghFluid.set
    (
        i,
        new volScalarField
        (
            "gh",
            (gFluid[i] & fluidRegions[i].C()) - ghRef
        )
    );

    Info<< "    Adding to ghfFluid\n" << endl;
    ghfFluid.set
    (
        i,
        new surfaceScalarField
        (
            "ghf",
            (gFluid[i] & fluidRegions[i].Cf()) - ghRef
        )
    );

    Info<< "    Adding to turbulence\n" << endl;
    turbulence.set
    (
        i,
        compressible::turbulenceModel::New
        (
            rhoFluid[i],
            UFluid[i],
            phiFluid[i],
            thermoFluid[i]
        ).ptr()
    );

    p_rghFluid.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "p_rgh",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );

    // CHANGES TO INITIAL SOLUTION

   
    specie theSpecie( thermoFluid[i].subDict("mixture") );
    dimensionedScalar RperfectGas ("RperfectGas",dimensionSet(0,2,-2,-1,0,0,0),scalar(theSpecie.R())); 


    // Force p_rgh to be consistent with p
    //p_rghFluid[i] = thermoFluid[i].p() - rhoFluid[i]*ghFluid[i];
    //rhoFluid[i] = ;
    thermoFluid[i].p() = p_rghFluid[i] + p_rghFluid[i] /  ( RperfectGas * thermoFluid[i].T() - ghFluid[i] ) *ghFluid[i];
    thermoFluid[i].correct();

    fluidRegions[i].setFluxRequired(p_rghFluid[i].name());

    radiation.set
    (
        i,
        radiation::radiationModel::New(thermoFluid[i].T())
    );

    initialMassFluid[i] = fvc::domainIntegrate(rhoFluid[i]).value();

    const dictionary& simpleDict =
        fluidRegions[i].solutionDict().subDict("SIMPLE");

    setRefCell
    (
        thermoFluid[i].p(),
        p_rghFluid[i],
        simpleDict,
        pRefCellFluid[i],
        pRefValueFluid[i]
    );

    simpleDict.readIfPresent("frozenFlow", frozenFlowFluid[i]);

    rhoMax.set
    (
        i,
        new dimensionedScalar
        (
            dimensionedScalar::lookupOrDefault
            (
                "rhoMax",
                simpleDict,
                dimDensity,
                GREAT
            )
        )
    );

    rhoMin.set
    (
        i,
        new dimensionedScalar
        (
            dimensionedScalar::lookupOrDefault
            (
                "rhoMin",
                simpleDict,
                dimDensity,
                0
            )
        )
    );

    Info<< "    Adding MRF\n" << endl;
    MRFfluid.set
    (
        i,
        new IOMRFZoneList(fluidRegions[i])
    );

    Info<< "    Adding fvOptions\n" << endl;
    fluidFvOptions.set
    (
        i,
        new fv::IOoptionList(fluidRegions[i])
    );

    Info<< "\nInitial Solution for " << fluidRegions[i].name() << endl;

    inlet     = fluidRegions[i].boundaryMesh().findPatchID("Inlet"); 
    outlet    = fluidRegions[i].boundaryMesh().findPatchID("Outlet");

    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"  << "  ClockTime = " << runTime.elapsedClockTime() << " s"  << endl;

    Info<<  fluidRegions[i].name() <<"  Inflow      : "   << -1.0* sum( phiFluid[i].boundaryField()[inlet])   <<" [kg/s]" << endl;
    Info<<  fluidRegions[i].name() <<"  Outflow     : "   <<       sum( phiFluid[i].boundaryField()[outlet])  <<" [kg/s]" <<  endl;
    Info<<  fluidRegions[i].name() <<"  EnergyInflow  : " << -1.0* sum( phiFluid[i].boundaryField()[inlet]  * ( thermoFluid[i].he().boundaryField()[inlet]  + 0.5*magSqr(UFluid[i].boundaryField()[inlet])  ) )   <<" [W]"  <<  endl;
    Info<<  fluidRegions[i].name() <<"  EnergyOutflow : " <<       sum( phiFluid[i].boundaryField()[outlet] * ( thermoFluid[i].he().boundaryField()[outlet] + 0.5*magSqr(UFluid[i].boundaryField()[outlet]) ) )   <<" [W]"  <<  endl;   
    
    Info<<  fluidRegions[i].name() <<"  rho max/avg/min : " << gMax(thermoFluid[i].rho()) << " " << gAverage(thermoFluid[i].rho())  << " " << gMin(thermoFluid[i].rho()) << endl;
    Info<<  fluidRegions[i].name() <<"  T   max/avg/min : " << gMax(thermoFluid[i].T())   << " " << gAverage(thermoFluid[i].T())    << " " << gMin(thermoFluid[i].T())   << endl;
    Info<<  fluidRegions[i].name() <<"  P   max/avg/min : " << gMax(thermoFluid[i].p())   << " " << gAverage(thermoFluid[i].p())    << " " << gMin(thermoFluid[i].p())   << endl;
    Info<<  fluidRegions[i].name() <<"  Prg max/avg/min : " << gMax(p_rghFluid[i])        << " " << gAverage(p_rghFluid[i])         << " " << gMin(p_rghFluid[i])        << endl;
    Info<<  fluidRegions[i].name() <<"  U   max/avg/min : " << max(UFluid[i].component(2)).value() << " " <<  average(UFluid[i].component(2)).value() << " " <<  min(UFluid[i].component(2)).value() << endl;

    Info<<  fluidRegions[i].name() <<"  K average:  " << average(thermoFluid[i].kappa()).value()  << endl;
    Info<<  fluidRegions[i].name() <<"  nu average: " << average(thermoFluid[i].nu()).value()     << endl;
}