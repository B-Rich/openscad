// example025 rendering an image via surface.

render_obj=7;

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
  scale([0.1,0.1,1]) surface(file="example025.png",center=true,convexity=5,scale=0.7);
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
  rotate([90,0,0]) scale([1,1,4]) polyset(data=readImg);
  translate([0,0,-30]) polyset(data=readImg);
}

if(render_obj==5) {
  polyset(data=read_stl("example012.stl",4));
}

if(render_obj==6) {
  polyset(data=read_dxf("example008.dxf","G"),convexity=2);
}

tmp_rgb=read_rgb();
if(render_obj==7) assign(readImg=read_rgb("example025_10x10.png")) {
  polyset(data=readImg,convexity=2);
  echo(str("read_rgb: ",readImg));
  translate([-2,-2,0]) cube();
}
