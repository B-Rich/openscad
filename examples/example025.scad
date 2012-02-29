// example025 rendering an image via surface.

render_obj=6;

if(render_obj==1) {
  intersection()
  {
	surface(file = "example010.dat",
		center = true, convexity = 5);
	
	rotate(45, [0, 0, 1])
	surface(file = "example010.dat",
		center = true, convexity = 5
		, reconstruct=true);
  }
}

if(render_obj==2) {
  	surface(file = "example010.dat",
		center = true, convexity = 5
		, reconstruct=true);
}

if(render_obj==3) {
  scale([0.1,0.1,1]) surface(read=read_image("example025.png",true,0.5),center=true,convexity=5,scale=0.5,reconstruct=true);
}

// echo usage
tmp_image=read_image();
tmp_dxf=read_dxf();
tmp_stl=read_stl();

if(render_obj==4) assign(readImg=read_image("example025.png",true,0.5) ) {
  echo(str("Number of polygons: ",len(readImg)));
  echo(str("  First polygon: ",readImg[0]));
  //numPolys=len(readImg);
  echo(str("  Last polygon: ", readImg[ (len(readImg)-1) ] ) );
  echo(str("  Bounds check: ", readImg[ len(readImg) ] ) );
  rotate([90,0,0]) scale([1,1,4]) surface(read=readImg);
  translate([0,0,-30]) surface(read=readImg);
}

if(render_obj==5) {
  surface(read=read_stl("example012.stl",5));
}

if(render_obj==6) {
  surface(read=read_dxf("example008.dxf","G"));
}
