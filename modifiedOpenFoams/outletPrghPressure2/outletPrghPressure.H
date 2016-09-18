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
Class
    Foam::outletPrghPressureFvPatchScalarField
Group
    grpOutletBoundaryConditions
Description
    This boundary condition provides an outflow condition for total
    temperature for use with supersonic cases, where a user-specified
    value is applied in the case of reverse flow.
Usage
    \table
        Property     | Description             | Required    | Default value
        U            | velocity field name     | no          | U
        phi          | flux field name         | no          | phi
        psi          | compressibility field name |  no      | psi
        gamma        | heat capacity ration (Cp/Cv) | yes    |
        inletValue   | reverse flow (inlet) value | yes      |
        T0           | static temperature [K]  | yes         |
    \endtable
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            outletPrghPressure;
        U               U;
        phi             phi;
        psi             psi;
        gamma           gamma;
        inletValue      uniform 0;
        T0              uniform 0;
        value           uniform 0;
    }
    \endverbatim
See also
    Foam::inletOutletFvPatchField
SourceFiles
    outletPrghPressureFvPatchScalarField.C
\*---------------------------------------------------------------------------*/

#ifndef outletPrghPressureFvPatchScalarField_H
#define outletPrghPressureFvPatchScalarField_H

#include "inletOutletFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
       Class outletPrghPressureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class outletPrghPressureFvPatchScalarField
:
    public inletOutletFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("outletPrghPressure");


    // Constructors

        //- Construct from patch and internal field
        outletPrghPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        outletPrghPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  outletPrghPressureFvPatchScalarField onto a new patch
        outletPrghPressureFvPatchScalarField
        (
            const outletPrghPressureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        outletPrghPressureFvPatchScalarField
        (
            const outletPrghPressureFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new outletPrghPressureFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        outletPrghPressureFvPatchScalarField
        (
            const outletPrghPressureFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new outletPrghPressureFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchScalarField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //