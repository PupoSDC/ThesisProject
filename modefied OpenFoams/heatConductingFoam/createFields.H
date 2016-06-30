Info<< "Reading thermophysical properties\n" << endl;

autoPtr<solidThermo> pThermo(solidThermo::New(mesh));
solidThermo& thermo = pThermo();
thermo.validate(args.executable(), "h", "e");

#include "readhRef.H"

Info<< "Reading fields\n" << endl;

volScalarField rho
( 
    IOobject("rho",runTime.timeName(),mesh,IOobject::NO_READ,IOobject::AUTO_WRITE),
    thermo.rho()
);