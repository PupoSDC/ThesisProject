{
    volScalarField& h = thermo.he();

    fvScalarMatrix EEqn
    (
         -fvm::laplacian(thermo.alpha(), h, "laplacian(alpha,h)") == fvOptions(rho, h)
    );

    EEqn.relax();

    fvOptions.constrain(EEqn);

    EEqn.solve();

    fvOptions.correct(h);
    thermo.correct();    
}
