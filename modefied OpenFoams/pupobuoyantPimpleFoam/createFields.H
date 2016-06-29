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

thermo.p()   = p_rgh * exp( gh / (  RperfectGas * thermo.T() ));
thermo.rho() = thermo.p() / ( RperfectGas * thermo.T() );

Info<< "rho max/avg/min : " << max(thermo.rho()).value() << " " << average(thermo.rho()).value() << " " << min(thermo.rho()).value() << endl;
Info<< "T   max/avg/min : " << max(thermo.T()).value()   << " " << average(thermo.T()).value()   << " " << min(thermo.T()).value()   << endl;
Info<< "P   max/avg/min : " << max(thermo.p()).value()   << " " << average(thermo.p()).value()   << " " << min(thermo.p()).value()   << endl;
Info<< "Prg max/avg/min : " << max(p_rgh).value()        << " " << average(p_rgh).value()        << " " << min(p_rgh).value()        << endl;
Info<< " " << endl;

#include "compressibleCreatePhi.H"

Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New(rho,U,phi,thermo)
);

mesh.setFluxRequired(p_rgh.name());
volScalarField& p = thermo.p();
const volScalarField& psi = thermo.psi();
rho = thermo.rho();

Info<< "Creating field dpdt\n" << endl;
volScalarField dpdt
(
    IOobject("dpdt",runTime.timeName(),mesh),
    mesh,dimensionedScalar("dpdt", p.dimensions()/dimTime, 0)
);

Info<< "Creating field kinetic energy K\n" << endl;
volScalarField K("K", 0.5*magSqr(U));
