#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# get animation scene
animationScene1 = GetAnimationScene()

animationScene1.GoToLast()

# get active source.
bugOpenFOAM = GetActiveSource()

# Properties modified on bugOpenFOAM
bugOpenFOAM.VolumeFields = ['p', 'U']

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [1001, 497]

# show data in view
bugOpenFOAMDisplay = Show(bugOpenFOAM, renderView1)
# trace defaults for the display properties.
bugOpenFOAMDisplay.ColorArrayName = [None, '']
bugOpenFOAMDisplay.GlyphType = 'Arrow'
bugOpenFOAMDisplay.ScalarOpacityUnitDistance = 0.08772182761234618

# reset view to fit data
renderView1.ResetCamera()

# set scalar coloring
ColorBy(bugOpenFOAMDisplay, ('FIELD', 'vtkBlockColors'))

# show color bar/color legend
bugOpenFOAMDisplay.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for 'vtkBlockColors'
vtkBlockColorsLUT = GetColorTransferFunction('vtkBlockColors')
vtkBlockColorsLUT.InterpretValuesAsCategories = 1
vtkBlockColorsLUT.Annotations = ['0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '8', '8', '9', '9', '10', '10', '11', '11']
vtkBlockColorsLUT.ActiveAnnotatedValues = ['0']
vtkBlockColorsLUT.IndexedColors = [1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.63, 0.63, 1.0, 0.67, 0.5, 0.33, 1.0, 0.5, 0.75, 0.53, 0.35, 0.7, 1.0, 0.75, 0.5]

# get opacity transfer function/opacity map for 'vtkBlockColors'
vtkBlockColorsPWF = GetOpacityTransferFunction('vtkBlockColors')

# create a new 'Glyph'
glyph1 = Glyph(Input=bugOpenFOAM,
    GlyphType='Arrow')
glyph1.Scalars = ['POINTS', 'None']
glyph1.Vectors = ['POINTS', 'None']
glyph1.ScaleFactor = 0.15000000000000002
glyph1.GlyphTransform = 'Transform2'

# Properties modified on glyph1
glyph1.Vectors = ['POINTS', 'U']
glyph1.ScaleMode = 'vector'
glyph1.GlyphMode = 'All Points'

# show data in view
glyph1Display = Show(glyph1, renderView1)
# trace defaults for the display properties.
glyph1Display.ColorArrayName = [None, '']
glyph1Display.GlyphType = 'Arrow'

# set scalar coloring
ColorBy(glyph1Display, ('FIELD', 'vtkBlockColors'))

# show color bar/color legend
glyph1Display.SetScalarBarVisibility(renderView1, True)

#### saving camera placements for all active views

# current camera placement for renderView1
renderView1.CameraPosition = [0.0072430993895977736, 0.0, 3.6478202735469933]
renderView1.CameraFocalPoint = [0.0072430993895977736, 0.0, 0.75]
renderView1.CameraParallelScale = 0.7500110760781583

#### uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).