<!-- Scroll-Ränder -->
<!-- links -->
<div id="l_scroll"
	style="position:absolute; left:0px; top:25%; width: 10px; height: 50%; z-index: 191; background: url('../pix/ui/elements/mapscrolling/left_off.png') left center no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(-10,0); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/left.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/left_off.png)';"></div>
<!-- rechts -->
<div id="r_scroll"
	 style="position:absolute; right:0px; top:25%; width: 10px; height: 50%; z-index: 191; background: url('../pix/ui/elements/mapscrolling/right_off.png') right center no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(10,0); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/right.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/right_off.png)';"></div>
<!-- oben -->
<div id="o_scroll"
	 style="position:absolute; left:25%; top:0px; width: 50%; height: 10px; z-index: 191;  background: url('../pix/ui/elements/mapscrolling/top_off.png') top center no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(0,-10); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/top.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/top_off.png)';"></div>
<!-- unten -->
<div id="u_scroll"
	 style="position:absolute; left:25%; bottom:0px; width: 50%; height: 10px; z-index: 191; background: url('../pix/ui/elements/mapscrolling/bottom_off.png') bottom center no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(0,10); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottom.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottom_off.png)';"></div>
<!-- ecke oben links -->
<div id="ol_scroll1"
	style="position:absolute; left:0px; top:0px; width: 15px; height: 25%; z-index: 191; background: url('../pix/ui/elements/mapscrolling/topleft_off.png') left top no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(-10,-10); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/topleft.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/topleft_off.png)';"></div>
<div id="ol_scroll2"
	style="position:absolute; left:0px; top:0px; width: 25%; height: 10px; z-index: 191;"
	onmouseover="javascript: MapUI.start_map_scroll(-10,-10); getElem('ol_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/topleft.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); getElem('ol_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/topleft_off.png)';"></div>
<!-- ecke oben rechts -->
<div id="or_scroll1"
	style="position:absolute; right:0px; top:0px; width: 15px; height: 25%; z-index: 191; background: url('../pix/ui/elements/mapscrolling/topright_off.png') right top no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(10,-10); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/topright.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/topright_off.png)';"></div>
<div id="or_scroll2"
	style="position:absolute; right:0px; top:0px; width: 25%; height: 20px; z-index: 191;"
	onmouseover="javascript: MapUI.start_map_scroll(10,-10); getElem('or_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/topright.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); getElem('or_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/topright_off.png)';"></div>
<!-- ecke unten rechts -->
<div id="ur_scroll1"
	style="position:absolute; right:0px; bottom:0px; width: 15px; height: 25%; z-index: 191; background: url('../pix/ui/elements/mapscrolling/bottomright_off.png') right bottom no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(10,10); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomright.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomright_off.png)';"></div>
<div id="ur_scroll2"
	style="position:absolute; right:0px; bottom:0px; width: 25%; height: 10px; z-index: 191;"
	onmouseover="javascript: MapUI.start_map_scroll(10,10); getElem('ur_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomright.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); getElem('ur_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomright_off.png)';"></div>
<!-- ecke unten links -->
<div id="ul_scroll1"
	style="position:absolute; left:0px; bottom:0px; width: 15px; height: 25%; z-index: 191; background: url('../pix/ui/elements/mapscrolling/bottomleft_off.png') left bottom no-repeat;"
	onmouseover="javascript: MapUI.start_map_scroll(-10,10); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomleft.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); this.style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomleft_off.png)';"></div>
<div id="ul_scroll2"
	style="position:absolute; left:0px; bottom:0px; width: 25%; height: 10px; z-index: 191;"
	onmouseover="javascript: MapUI.start_map_scroll(-10,10); getElem('ul_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomleft.png)';"
	onmouseout="javascript: MapUI.stop_map_scroll(); getElem('ul_scroll1').style.backgroundImage='url(../pix/ui/elements/mapscrolling/bottomleft_off.png)';"></div>