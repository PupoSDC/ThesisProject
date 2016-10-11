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
\*---------------------------------------------------------------------------*/

#include "totalPressureVelocity.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"
#include "surfaceFields.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::totalPressureVelocity::totalPressureVelocity
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(p, iF),
    p0_(p.size(), 0.0)
{}


Foam::totalPressureVelocity::totalPressureVelocity
(
    const totalPressureVelocity& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchVectorField(ptf, p, iF, mapper),
    p0_(ptf.p0_, mapper)
{}


Foam::totalPressureVelocity::totalPressureVelocity
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchVectorField(p, iF),
    p0_("p0", dict, p.size())
{
    if (dict.found("value"))
    {
        fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
    }
    else
    {
        FatalErrorInFunction
            << "hurr durr value"
            << "\n    on patch " << this->patch().name()
            << " of field " << this->internalField().name()
            << " in file " << this->internalField().objectPath()
            << exit(FatalError);
    }
}


Foam::totalPressureVelocity::totalPressureVelocity
(
    const totalPressureVelocity& pivpvf
)
:
    fixedValueFvPatchVectorField(pivpvf),
    p0_(pivpvf.p0_)
{}


Foam::totalPressureVelocity::totalPressureVelocity
(
    const totalPressureVelocity& pivpvf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchVectorField(pivpvf, iF),
    p0_(pivpvf.p0_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::totalPressureVelocity::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    tmp<vectorField> n = patch().nf();

    const fvPatchField<scalar>& rhop =
        patch().lookupPatchField<volScalarField, scalar>("rho");


    const fvPatchField<scalar>& pp =
        patch().lookupPatchField<volScalarField, scalar>("p");

    operator==(n* sqrt( 2*mag(p0_ - pp)/rhop ));

    fixedValueFvPatchVectorField::updateCoeffs();
}


void Foam::totalPressureVelocity::write(Ostream& os) const
{
    fvPatchVectorField::write(os);
    p0_.writeEntry("p0", os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void Foam::totalPressureVelocity::operator=
(
    const fvPatchField<vector>& pvf
)
{
    fvPatchField<vector>::operator=(patch().nf()*(patch().nf() & pvf));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchVectorField,
        totalPressureVelocity
    );
}

// ************************************************************************* //
