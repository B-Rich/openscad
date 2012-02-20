// Braille Example.

use <MCAD/fonts.scad>

8bit_polyfont_defaults=8bit_polyfont();
linear_extrude(height=4.0, center=true) polytext("OpenSCAD Rocks!",12,8bit_polyfont_defaults,justify=0,align=0,underline=true);

braille_ascii_font_defaults=braille_ascii_font();
echo(str("Default Braille ASCII Font (Spec 800) cell width: ",braille_ascii_font_defaults[0][0]));
echo(str("Default Braille ASCII Font (Spec 800) cell height: ",braille_ascii_font_defaults[0][1]));

translate([0,braille_ascii_font_defaults[0][1]])
  braille_ascii_spec800("Hello, World!",cell_backing=true,echo_translate=true);

translate([0,-braille_ascii_font_defaults[0][1]])
  braille_ascii_spec800(",Hello1 ,World6",justify=-1,align=1,cell_backing=true,echo_translate=true);
