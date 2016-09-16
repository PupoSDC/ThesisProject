#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# find source
bugOpenFOAM = FindSource('bug.OpenFOAM')

# create a new 'Glyph'
glyph1 = Glyph(Input=bugOpenFOAM,
    GlyphType='Arrow')
glyph1.Scalars = ['POINTS', 'None']
glyph1.Vectors = ['POINTS', 'U']
glyph1.ScaleFactor =  0.02
glyph1.ScaleMode = 'vector'
glyph1.GlyphMode = 'All Points'
glyph1.GlyphTransform = 'Transform2'

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [1611, 809]

# show data in view
glyph1Display = Show(glyph1, renderView1)
# trace defaults for the display properties.
glyph1Display.ColorArrayName = [None, '']
glyph1Display.GlyphType = 'Arrow'

# set scalar coloring
ColorBy(glyph1Display, ('FIELD', 'vtkBlockColors'))

# show color bar/color legend
glyph1Display.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for 'vtkBlockColors'
vtkBlockColorsLUT = GetColorTransferFunction('vtkBlockColors')

# get opacity transfer function/opacity map for 'vtkBlockColors'
vtkBlockColorsPWF = GetOpacityTransferFunction('vtkBlockColors')

# set scalar coloring
ColorBy(glyph1Display, ('POINTS', 'U'))

# rescale color and/or opacity maps used to include current data range
glyph1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
glyph1Display.SetScalarBarVisibility(renderView1, True)

