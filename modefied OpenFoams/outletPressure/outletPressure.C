/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016 OpenFOAM Foundation
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

#include "outletPressure.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "mappedPatchBase.H"
#include "uniformDimensionedFields.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::outletPressureFvPatchScalarField::outletPressureFvPatchScalarField
(const fvPatch& p,const DimensionedField<scalar, volMesh>& iF):
outletPressureFvPatchScalarField(p, iF)
{
    refValue()      = 0.0;
    refGrad()       = 0.0;
    valueFraction() = 0.0;
}

Foam::outletPressureFvPatchScalarField::outletPressureFvPatchScalarField
(const fvPatch& p,const DimensionedField<scalar, volMesh>& iF,const dictionary& dict):
outletPressureFvPatchScalarField(p, iF)
{
    refValue()      = 0.0;
    refGrad()       = 0.0;
    valueFraction() = 0.0;
    evaluate();
}

Foam::outletPressureFvPatchScalarField::outletPressureFvPatchScalarField
(const outletPressureFvPatchScalarField& ptf,const fvPatch& p,const DimensionedField<scalar, volMesh>& iF,const fvPatchFieldMapper& mapper):
outletPressureFvPatchScalarField(ptf, p, iF, mapper){}

Foam::outletPressureFvPatchScalarField::outletPressureFvPatchScalarField
(const outletPressureFvPatchScalarField& ptf):
outletPressureFvPatchScalarField(ptf){}

Foam::outletPressureFvPatchScalarField::outletPressureFvPatchScalarField
(const outletPressureFvPatchScalarField& ptf,const DimensionedField<scalar, volMesh>& iF):
outletPressureFvPatchScalarField(ptf, iF){}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::outletPressureFvPatchScalarField::autoMap
(const fvPatchFieldMapper& m)
{
    outletPressureFvPatchScalarField::autoMap(m);
}

void Foam::outletPressureFvPatchScalarField::rmap
(const fvPatchScalarField& ptf,const labelList& addr)
{
    mixedFvPatchScalarField::rmap(ptf, addr);
}


void Foam::outletPressureFvPatchScalarField::updateCoeffs()
{
    if (updated()){ return; }

    const fvPatchField<scalar>& rhop       = patch().lookupPatchField<volScalarField, scalar>("rho");
    const uniformDimensionedVectorField& g = db().lookupObject<uniformDimensionedVectorField>("g");

    refGrad()       = rhop * g;

    refValue()      = 0.0;
    valueFraction() = 0.0;

    outletPressureFvPatchScalarField::updateCoeffs();
}


void Foam::outletPressureFvPatchScalarField::write(Ostream& os) const
{
    fvPatchScalarField::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * Build Macro Function  * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        outletPressureFvPatchScalarField
    );
}

// ************************************************************************* //
