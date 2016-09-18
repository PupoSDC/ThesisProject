Info<< "Reading thermophysical properties\n" << endl;

autoPtr<rhoThermo> pThermo(rhoThermo::New(mesh));
rhoThermo& thermo = pThermo();
thermo.validate(args.executable(), "h", "e");

specie theSpecie( thermo.subDict("mixture") );
dimensionedScalar RperfectGas ("RperfectGas",dimensionSet(0,2,-2,-1,0,0,0),scalar(theSpecie.R())); 

#include "readGravitationalAcceleration.H"
#include "readhRef.H"
#include "gh.H"

thermo.p()  = thermo.p() * Foam::exp( gh / RperfectGas / thermo.T() ); 
thermo.correct();

volScalarField& p = thermo.p();

volVectorField U
(
    IOobject("U",runTime.timeName(),mesh,IOobject::MUST_READ,IOobject::AUTO_WRITE),
    mesh
);

volScalarField rho
(
    IOobject("rho",runTime.timeName(),mesh,IOobject::READ_IF_PRESENT,IOobject::AUTO_WRITE),
    thermo.rho()
);

surfaceScalarField phi
(
    IOobject("phi",runTime.timeName(),mesh,IOobject::READ_IF_PRESENT,IOobject::AUTO_WRITE),
    linearInterpolate(rho*U) & mesh.Sf()
);

surfaceScalarField snGradRho
(
    IOobject("snGradRho",runTime.timeName(),mesh,IOobject::READ_IF_PRESENT,IOobject::AUTO_WRITE),
    fvc::snGrad(rho)
);

autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New(rho,U,phi,thermo)
);

volScalarField p_rgh
(
    IOobject("p_rgh",runTime.timeName(),mesh,IOobject::MUST_READ,IOobject::AUTO_WRITE),
    mesh
);

p_rgh = p - rho*gh;

label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p,p_rgh,simple.dict(),pRefCell,pRefValue);

mesh.setFluxRequired(p_rgh.name());

#include "createMRF.H"
#include "createRadiationModel.H"

label inlet  = mesh.boundaryMesh().findPatchID("Inlet"); 
label outlet = mesh.boundaryMesh().findPatchID("Outlet");
    
Info<< " " << endl;
Info<< "rho max/avg/min : " << gMax(thermo.rho())   << " " << gAverage(thermo.rho())   << " " << gMin(thermo.rho())   << endl;
Info<< "T   max/avg/min : " << gMax(thermo.T())     << " " << gAverage(thermo.T())     << " " << gMin(thermo.T())     << endl;
Info<< "P   max/avg/min : " << gMax(thermo.p())     << " " << gAverage(thermo.p())     << " " << gMin(thermo.p())     << endl;
Info<< "Prg max/avg/min : " << gMax(p_rgh)          << " " << gAverage(p_rgh)          << " " << gMin(p_rgh)          << endl;
Info<< "U   max/avg/min : " << max(U.component(2)).value() << " " <<  average(U.component(2)).value() << " " <<  min(U.component(2)).value() << endl;
Info<< " " << endl;
Info<< "sngrad(rho) :" << gMax(snGradRho.boundaryField()[outlet]) << " " << gAverage(snGradRho.boundaryField()[outlet]) << " " << gMin(snGradRho.boundaryField()[outlet]) << endl;
Info<< " " << endl;