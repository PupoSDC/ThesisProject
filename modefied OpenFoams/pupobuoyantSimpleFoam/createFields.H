Info<< "Reading thermophysical properties\n" << endl;

autoPtr<rhoThermo> pThermo(rhoThermo::New(mesh));
rhoThermo& thermo = pThermo();
thermo.validate(args.executable(), "h", "e");

#include "readGravitationalAcceleration.H"
#include "readhRef.H"
#include "gh.H"

specie theSpecie( thermo.subDict("mixture") );
dimensionedScalar RperfectGas ("RperfectGas",dimensionSet(0,2,-2,-1,0,0,0),scalar(theSpecie.R())); 

Info<< "Reading fields\n" << endl;

volScalarField p_rgh
(
    IOobject("p_rgh",runTime.timeName(),mesh,IOobject::MUST_READ,IOobject::AUTO_WRITE),
    mesh
);

volScalarField rho
( 
    IOobject("rho",runTime.timeName(),mesh,IOobject::NO_READ,IOobject::AUTO_WRITE),
    thermo.rho()
);

volVectorField U
(
    IOobject("U",runTime.timeName(),mesh,IOobject::MUST_READ,IOobject::AUTO_WRITE),
    mesh
);

thermo.rho() = p_rgh / (  RperfectGas * thermo.T() - gh );
thermo.p()   = p_rgh + thermo.rho()*gh;

Info<< "rho max/avg/min : " << gMax(thermo.rho())   << " " << gAverage(thermo.rho())   << " " << gMin(thermo.rho())   << endl;
Info<< "T   max/avg/min : " << gMax(thermo.T())     << " " << gAverage(thermo.T())     << " " << gMin(thermo.T())     << endl;
Info<< "P   max/avg/min : " << gMax(thermo.p())     << " " << gAverage(thermo.p())     << " " << gMin(thermo.p())     << endl;
Info<< "Prg max/avg/min : " << gMax(p_rgh)          << " " << gAverage(p_rgh)          << " " << gMin(p_rgh)          << endl;
Info<< "U   max/avg/min : " << max(U.component(2)).value() << " " <<  average(U.component(2)).value() << " " <<  min(U.component(2)).value() << endl;

#include "compressibleCreatePhi.H"

Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New(rho,U,phi,thermo)
);

//label pRefCell = 0;
scalar pRefValue = gMax(p_rgh);

mesh.setFluxRequired(p_rgh.name());
volScalarField& p = thermo.p();
rho = thermo.rho();

dimensionedScalar initialMass = fvc::domainIntegrate(rho);
dimensionedScalar totalVolume = sum(mesh.V());
