Info<< "Reading thermophysical properties\n" << endl;

autoPtr<rhoThermo> pThermo(rhoThermo::New(mesh));
rhoThermo& thermo = pThermo();
thermo.validate(args.executable(), "h", "e");

specie theSpecie( thermo.subDict("mixture") );
dimensionedScalar RperfectGas ("RperfectGas",dimensionSet(0,2,-2,-1,0,0,0),scalar(theSpecie.R())); 

#include "readGravitationalAcceleration.H"
#include "readhRef.H"
#include "gh.H"

thermo.p() = thermo.p() * Foam::exp( gh / RperfectGas / thermo.T() ); 
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
    
Info<< "rho max/avg/min : " << gMax(thermo.rho())   << " " << gAverage(thermo.rho())    << " " << gMin(thermo.rho())   << endl;
Info<< "T   max/avg/min : " << gMax(thermo.T())     << " " << gAverage(thermo.T())      << " " << gMin(thermo.T())     << endl;
Info<< "P   max/avg/min : " << gMax(thermo.p())     << " " << gAverage(thermo.p())      << " " << gMin(thermo.p())     << endl;
Info<< "Prg max/avg/min : " << gMax(p_rgh)          << " " << gAverage(p_rgh)           << " " << gMin(p_rgh)          << endl;
Info<< "U   max/avg/min : " << gMax(U).component(2) << " " << gAverage(U).component(2)  << " " << gMin(U).component(2) << endl;
Info<< "Prg max-min : "  << gMax(p_rgh)  -  gMin(p_rgh)        << endl;
Info<< " " << endl;