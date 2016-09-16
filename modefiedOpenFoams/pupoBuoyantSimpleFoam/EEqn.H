{
    volScalarField& he  = thermo.he();
    volScalarField& rho = thermo.rho();

    fvScalarMatrix EEqn
    (
        fvm::div(phi, he) 
      + fvc::div(phi, volScalarField("K", 0.5*magSqr(U))) 
      - fvm::laplacian(turbulence->alphaEff(), he)
     ==
        thermo.rho()*(U&g)
      + radiation->Sh(thermo)
      + fvOptions(rho, he)
    );

    EEqn.relax();

    fvOptions.constrain(EEqn);

    EEqn.solve();

    fvOptions.correct(he);

    thermo.correct();
    radiation->correct();
    snGradRho = fvc::snGrad(rho);
}
