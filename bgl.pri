bgl {
  DEFINES += ENABLE_BGL

  # Optionally specify location of OpenCSG using the 
  # OPENCSGDIR env. variable
  BGL_DIR = $$(BGLDIR)
  !isEmpty(BGL_DIR) {
    QMAKE_INCDIR += $$BGL_DIR/include
    QMAKE_LIBDIR += $$BGL_DIR/lib
    message("BGL location: $$BGL_DIR")
  }

  LIBS += -lBGL
}
