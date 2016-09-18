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

#include "outletPrghPressure.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H" 
#include "surfaceFields.H"
#include "uniformDimensionedFields.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::outletPrghPressureFvPatchScalarField::
outletPrghPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    inletOutletFvPatchScalarField(p, iF)
{
    this->refValue() = patchInternalField();
    this->refGrad() = Zero;
    this->valueFraction() = 0.0;
}


Foam::outletPrghPressureFvPatchScalarField::
outletPrghPressureFvPatchScalarField
(
    const outletPrghPressureFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    inletOutletFvPatchScalarField(ptf, p, iF, mapper)
{}


Foam::outletPrghPressureFvPatchScalarField::
outletPrghPressureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    inletOutletFvPatchScalarField(p, iF)
{  
    this->refValue()      = scalarField("value", dict, p.size());    
    this->refGrad()       = Zero;
    this->valueFraction() = 0.0;
}


Foam::outletPrghPressureFvPatchScalarField::
outletPrghPressureFvPatchScalarField
(
    const outletPrghPressureFvPatchScalarField& tppsf
)
:
    inletOutletFvPatchScalarField(tppsf)
{}


Foam::outletPrghPressureFvPatchScalarField::
outletPrghPressureFvPatchScalarField
(
    const outletPrghPressureFvPatchScalarField& tppsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    inletOutletFvPatchScalarField(tppsf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::outletPrghPressureFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    inletOutletFvPatchScalarField::autoMap(m);
}


void Foam::outletPrghPressureFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    inletOutletFvPatchScalarField::rmap(ptf, addr);
}


void Foam::outletPrghPressureFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const fvsPatchField<scalar>& snGradRhop =
        patch().lookupPatchField<surfaceScalarField, scalar>("snGradRho");

    const fvsPatchField<scalar>& ghfp =
        patch().lookupPatchField<surfaceScalarField, scalar>("ghf");

    this->refGrad() = -1.0 * snGradRhop * ghfp;

    inletOutletFvPatchScalarField::updateCoeffs();
}


void Foam::outletPrghPressureFvPatchScalarField::write(Ostream& os)
const
{
    fvPatchScalarField::write(os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        outletPrghPressureFvPatchScalarField
    );
}

// ************************************************************************* //