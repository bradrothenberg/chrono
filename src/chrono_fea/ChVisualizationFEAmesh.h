//
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2013 Project Chrono
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file at the top level of the distribution
// and at http://projectchrono.org/license-chrono.txt.
//
// File author: A. Tasora

#ifndef CHVISUALIZATIONFEAMESH_H
#define CHVISUALIZATIONFEAMESH_H

#include "chrono/assets/ChAssetLevel.h"
#include "chrono/assets/ChColor.h"
#include "chrono/geometry/ChCTriangleMeshConnected.h"
#include "chrono_fea/ChMesh.h"
#include "chrono_fea/ChNodeFEAxyz.h"
#include "chrono_fea/ChNodeFEAxyzP.h"
#include "chrono_fea/ChNodeFEAxyzrot.h"

namespace chrono {
namespace fea {

/// Class for a FEA mesh visualization.
/// It converts tetahedrons, etc. into a coloured triangle mesh asset
/// of class ChTriangleMeshShape that is contained in its sublevel,
/// so that it can be rendered or postprocessed.

class ChApiFea ChVisualizationFEAmesh : public ChAssetLevel {
  public:
	  enum eChFemDataType {
		  E_PLOT_NONE = 0,
		  E_PLOT_SURFACE = 1,
		  E_PLOT_CONTACTSURFACES = 2,
		  E_PLOT_LOADSURFACES = 3,
		  E_PLOT_NODE_DISP_NORM = 4,
		  E_PLOT_NODE_DISP_X = 5,
		  E_PLOT_NODE_DISP_Y = 6,
		  E_PLOT_NODE_DISP_Z = 7,
		  E_PLOT_NODE_SPEED_NORM = 8,
		  E_PLOT_NODE_SPEED_X = 9,
		  E_PLOT_NODE_SPEED_Y = 10,
		  E_PLOT_NODE_SPEED_Z = 11,
		  E_PLOT_NODE_ACCEL_NORM = 12,
		  E_PLOT_NODE_ACCEL_X = 13,
		  E_PLOT_NODE_ACCEL_Y = 14,
		  E_PLOT_NODE_ACCEL_Z = 15,
		  E_PLOT_ELEM_STRAIN_VONMISES = 16,
		  E_PLOT_ELEM_STRESS_VONMISES = 17,
		  E_PLOT_ELEM_STRAIN_HYDROSTATIC = 18,
		  E_PLOT_ELEM_STRESS_HYDROSTATIC = 19,
		  E_PLOT_ELEM_BEAM_MX = 20,
		  E_PLOT_ELEM_BEAM_MY = 21,
		  E_PLOT_ELEM_BEAM_MZ = 22,
		  E_PLOT_ELEM_BEAM_TX = 23,
		  E_PLOT_ELEM_BEAM_TY = 24,
		  E_PLOT_ELEM_BEAM_TZ = 25,
		  E_PLOT_ELEM_BEAM_VON_MISES = 26,
		  E_PLOT_NODE_P = 27,  // scalar field for Poisson problems (ex. temperature if thermal FEM)
		  E_PLOT_ANCF_BEAM_AX = 28,
		  E_PLOT_ANCF_BEAM_BD = 29,
	  };
    enum eChFemGlyphs {
        E_GLYPH_NONE,
        E_GLYPH_NODE_DOT_POS,
        E_GLYPH_NODE_CSYS,
        E_GLYPH_NODE_VECT_SPEED,
        E_GLYPH_NODE_VECT_ACCEL,
        E_GLYPH_ELEM_TENS_STRAIN,
        E_GLYPH_ELEM_TENS_STRESS,
        E_GLYPH_ELEM_VECT_DP,  // gradient field for Poisson problems (ex. heat flow if thermal FEM)
    };

  protected:
    //
    // DATA
    //

    ChMesh* FEMmesh;

    eChFemDataType fem_data_type;
    eChFemGlyphs fem_glyph;

    double colorscale_min;
    double colorscale_max;

    double symbols_scale;
    double symbols_thickness;

    bool shrink_elements;
    double shrink_factor;

    bool wireframe;

    bool zbuffer_hide;

    bool smooth_faces;

    bool undeformed_reference;

    int beam_resolution;
    int beam_resolution_section;
    int shell_resolution;

    ChColor meshcolor;
    ChColor symbolscolor;

    std::vector<int> normal_accumulators;

  public:
    //
    // CONSTRUCTORS
    //

    ChVisualizationFEAmesh(ChMesh& mymesh);

    virtual ~ChVisualizationFEAmesh() {}

    //
    // FUNCTIONS
    //

    // Access the referenced FEM mesh
    virtual ChMesh& GetMesh() { return *FEMmesh; }

    // Returns the current data type to be plotted (speeds, forces, etc.)
    eChFemDataType GetFEMdataType() { return fem_data_type; }

    // Set the current data type to be plotted (speeds, forces, etc.)
    void SetFEMdataType(eChFemDataType mdata) { fem_data_type = mdata; }

    // Returns the current data type to be drawn with glyphs
    eChFemGlyphs GetFEMglyphType() { return fem_glyph; }

    // Set the current data type to be drawn with glyphs
    void SetFEMglyphType(eChFemGlyphs mdata) { fem_glyph = mdata; }

    // Set upper and lower values of the plotted variable for the colorscale plots.
    void SetColorscaleMinMax(double mmin, double mmax) {
        colorscale_min = mmin;
        colorscale_max = mmax;
    }

    // Set the scale for drawing the vector/tensors/etc. symbols
    void SetSymbolsScale(double mscale) { this->symbols_scale = mscale; }
    double GetSymbolsScale() { return this->symbols_scale; }

    // Set the thickness of symbols used for drawing the vector/tensors/etc.
    void SetSymbolsThickness(double mthick) { this->symbols_thickness = mthick; }
    double GetSymbolsThickness() { return this->symbols_thickness; }

    /// Set the resolution of beam triangulated drawing, along direction of beam
    void SetBeamResolution(int mres) { this->beam_resolution = mres; }
    int GetBeamResolution() { return this->beam_resolution; }

    /// Set the resolution of beam triangulated drawing, along the section 
    /// (i.e. for circular section= number of points along the circle)
    void SetBeamResolutionSection(int mres) { this->beam_resolution_section = mres; }
    int GetBeamResolutionSection() { return this->beam_resolution_section; }

    /// Set the resolution of shell triangulated drawing
    void SetShellResolution(int mres) { this->shell_resolution = mres; }
    int GetShellResolution() { return this->shell_resolution; }

    // Set shrinkage of elements during drawing
    void SetShrinkElements(bool mshrink, double mfact) {
        this->shrink_elements = mshrink;
        shrink_factor = mfact;
    }

    // Set as wireframe visualization
    void SetWireframe(bool mwireframe) { this->wireframe = mwireframe; }

    // Set the Z buffer enable/disable (for those rendering systems that can do this)
    // If hide= false, symbols will appear even if hidden by meshes/geometries. Default true.
    void SetZbufferHide(bool mhide) { this->zbuffer_hide = mhide; }

    // Set color for E_PLOT_SURFACE mode (also for wireframe lines)
    void SetDefaultMeshColor(ChColor mcolor) { this->meshcolor = mcolor; }

    // Set color for E_GLYPHS_NONE mode or for wireframe lines
    void SetDefaultSymbolsColor(ChColor mcolor) { this->symbolscolor = mcolor; }

    // Activate Gourad or Phong smoothing for faces of non-straight elements
    // (with a small performance overhead) -NOTE: experimental
    void SetSmoothFaces(bool msmooth) { this->smooth_faces = msmooth; }

    // If this flag is turned on, the mesh is drawn as it is
    // undeformed (the reference position).
    void SetDrawInUndeformedReference(bool mdu) { this->undeformed_reference = mdu; }

    // Updates the triangle visualization mesh so that it matches with the
    // FEM mesh (ex. tetrahedrons are converted in 4 surfaces, etc.
    virtual void Update(ChPhysicsItem* updater, const ChCoordsys<>& coords);

  private:
    double ComputeScalarOutput(std::shared_ptr<ChNodeFEAxyz> mnode, int nodeID, std::shared_ptr<ChElementBase> melement);
    double ComputeScalarOutput(std::shared_ptr<ChNodeFEAxyzP> mnode, int nodeID, std::shared_ptr<ChElementBase> melement);
    ChVector<float> ComputeFalseColor(double in);
    ChColor ComputeFalseColor2(double in);
};

}  // END_OF_NAMESPACE____
}  // END_OF_NAMESPACE____

#endif
