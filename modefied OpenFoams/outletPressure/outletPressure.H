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

Class
    Foam::outletPressure

Group
    grpGenericBoundaryConditions

Description

Usage

SourceFiles
    outletPressure.C

\*---------------------------------------------------------------------------*/

#ifndef outletPressureFvPatchScalarField_H
#define outletPressureFvPatchScalarField_H

#include "mixedFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
     Class outletPressureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class outletPressureFvPatchScalarField
:
   public mixedFvPatchScalarField
{      
public:

    //- Runtime type information
    TypeName("outletPressure");


    // outletPressureors

        //- Contruct from patch and internal field
        outletPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Contruct from patch, internal field and dictionary
        outletPressureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Contruct by mapping given BASETypeFvPatchField
        //  onto a new patch
        outletPressureFvPatchScalarField
        (
            const outletPressureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Contruct as copy
        outletPressureFvPatchScalarField
        (
            const outletPressureFvPatchScalarField&
        );

        //- Contruct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new outletPressureFvPatchScalarField(*this)
            );
        }

        //- contruct as copy setting internal field reference
        outletPressureFvPatchScalarField
        (
            const outletPressureFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new outletPressureFvPatchScalarField(*this,iF)
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

            virtual void updateCoeffs();
            
            virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
