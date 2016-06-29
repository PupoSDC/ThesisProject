{
    volScalarField& he = thermo.he();

    fvScalarMatrix EEqn
    (
        fvm::div(phi, he) + fvc::div(phi, volScalarField("K", 0.5*magSqr(U)))
        - fvm::laplacian(turbulence->alphaEff(), he)
     ==
        rho*(U&g)
      + radiation->Sh(thermo)
      + fvOptions(rho, he)
    );

    EEqn.relax();

    fvOptions.constrain(EEqn);

    EEqn.solve();

    fvOptions.correct(he);

    thermo.correct();
    radiation->correct();

}
