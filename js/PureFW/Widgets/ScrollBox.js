/**
 * Eine Box mit eigener JS-Scrollbar o.ä.
 * 
 * @author Phil
 * @deprecated
 */
 
PureFW.ScrollBox = function(parent_node, x, y, w, h, vertical) {
	this.node = null;
	this.container = null;
	this.content = null;
	this.scrollbar = null;
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h);
		this.vertical = vertical ? !!vertical : false;
		this.scrollbar_width = 15;
		
		this.instance_num = this.constructor.num_instances;
		this.id = 'Scrollbox'+this.instance_num;
		this.content_id = 'Scrollbox_cont'+this.instance_num;
		this.bg_img_id = 'Scrollbox_bg_img'+this.instance_num;
			
		this.insert_into_dom_tree();
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * ScrollBox extends Widget
 ********************************************************************/
PureFW.ScrollBox.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ScrollBox.num_instances = 0;		// Instanzüberwachung (Anzahl)
PureFW.ScrollBox.instances = new Array();	// Instanzüberwachung
 
/********************************************************************
 * Prototype-Funktionen
 ********************************************************************/

/**
 * Fügt die ScrollBox in den DOM-Baum ein.
 */
PureFW.ScrollBox.prototype.insert_into_dom_tree = function() {
	PureFW.ScrollBox.parent.insert_into_dom_tree.call(this);
	var w = this.get_width();
	var h = this.get_height();
	if (this.vertical)
		w -= this.get_scrollbar_width();
	else
		h -= this.get_scrollbar_width();
	
	this.container = new PureFW.Container(this.node, 0, 0, 
				this.get_width(), this.get_height());
	var container_style = this.container.get_node().style
	container_style.overflow = 'hidden';
	
	if (this.vertical) {
		this.content = new PureFW.Container(this.container.get_node(), 0, 0, w, 0);
		this.scrollbar = new PureFW.Scrollbar(this.container.get_node(), w, 0, 
				this.get_scrollbar_width(), this.get_height(), true);
	}
	else {
		this.content = new PureFW.Container(this.container.get_node(), 0, 0, 0, h);
		this.scrollbar = new PureFW.Scrollbar(this.container.get_node(), 0, h, 
				this.get_scrollbar_width(), this.get_width(), false);
	}
};

/**
 * Gibt die Größe der Scrollbar zurück
 */
PureFW.ScrollBox.prototype.get_scrollbar_width = function() {
	return this.scrollbar_width*this.scale_factor;
};

/**
 * Setzt den Inhalt (HTML) der Scrollbox. Wird interprete_js angegeben, wird
 * ggf. mitgesandtes JS (innerhalb der Tags <script></script>) interpretiert!
 * 
 * @param string text
 * @param bool interprete_js [=false]
 */
PureFW.ScrollBox.prototype.set_content = function(text, interprete_js) {
	this.on_change();
	this.content.set_content(text, interprete_js);
};

/**
 * Fügt der Scrollbox den Inhalt (HTML ohne Javascript) hinzu.
 * 
 * @param string text
 */
PureFW.ScrollBox.prototype.add_content = function(text) {
	this.on_change();
	this.content.add_content(text);
};

/**
 * Gibt den Inhalt (HTML) der Scrollbox zurück
 */
PureFW.ScrollBox.prototype.get_content = function() {
	return this.content.get_content();
}

PureFW.ScrollBox.prototype.get_content_id = function() {
	return this.content.id;
};
/**
 * Setzt, wie viele Pixel pro Scroll-Event gescrollt werden soll.
 * 
 * @param uint step
 * @see PureFW.Scrollbar.set_scroll_step
 */
PureFW.ScrollBox.prototype.set_scroll_step = function(step) {
	this.scrollbar.set_scroll_step(step);
};
/**
 * Setzt, nach wie vielen Millisekunden das Scroll-Event wieder aufgerufen
 * werden soll, wenn der Nutzer die Maustaste gedrückt hält.
 * 
 * @param uint interval
 * @see PureFW.Scrollbar.set_scroll_interval
 */
PureFW.ScrollBox.prototype.set_scroll_interval = function(interval) {
	this.scrollbar.set_scroll_interval(interval);
};

/**
 * Ob die Scrollsprünge zusätzlich animiert werden sollen. Macht nur Sinn,
 * wenn scroll_interval und scroll_step entsprechend hoch sind. So kann man
 * animiert zu einem gewissen Punkt springen.
 * 
 * @param bool ani
 * @see PureFW.Scrollbar.set_animate_scroll
 */
PureFW.ScrollBox.prototype.set_animate_scroll = function(ani) {
	this.scrollbar.set_animate_scroll(ani);
};
/**
 * Die Breite des Contents muss angegeben werden (und nur dann), wenn
 * horizontal gescrollt werden soll. Bei vertikalem Scrollen geschieht die
 * Größenermittlung automatisch.
 * 
 * @param uint w
 */
PureFW.ScrollBox.prototype.set_content_width = function(w) {
	this.content.set_width(w);
};
PureFW.ScrollBox.prototype.on_scroll = function(ev) {
	var content_style = document.getElemById(this.content_id).style;
	var step = ev.detail;
	if (this.vertical) {
		var top = this.content.get_x();
		var min_top = this.content.get_height()*(-1)+this.get_height();
		if ((step > 0) && (top > min_top)) {
			top -= step;
			if (top < min_top)
				top = min_top;
		}
		else if ((step < 0) && (top < 0)) {
			top -= step;
			if (top > 0)
				top = 0;
		}
		else {
			this.scroll_bar.stop_scroll();
			return;
		}
		this.content.set_x(top);
	}
	else {
		var left = this.content.get_y();
		var min_left = this.content.get_width()+this.get_width();
		if ((step > 0) && (left > min_left)) {
			left -= step;
			if (left < min_left)
				left = min_left;
		}
		else if ((step < 0) && (left < 0)) {
			left -= step;
			if (left > 0)
				left = 0;
		}
		else {
			this.scroll_bar.stop_scroll();
			return;
		}
		this.content.set_y(left);
	}
};
//	scroll_minus:function() {
//		var content_style = document.getElemById(this.content_id).style;
//		if (this.vertical) {
//			var top = isNaN(content_style.top) ? 0 : parseInt(content_style.top);
//			if (top < 0) {
//				top += step;
//				if (top > 0)
//					top = 0;
//				content_style.top = top+'px';
//			}
//			else {
//				this._scroll_minus_stop();
//				this._scroll_ani_minus_stop();
//			}			
//		}
//		else {
//			var left = isNaN(content_style.left) ? 0 : parseInt(content_style.left);
//			if (left < 0) {
//				left += step;
//				if (left > 0)
//					left = 0;
//				content_style.left = left+'px';
//			}
//			else {
//				this._scroll_minus_stop();
//				this._scroll_ani_minus_stop();
//			}			
//		}
//	},

PureFW.ScrollBox.prototype.destroy = function() {
	this.content.destroy();
	this.scrollbar.destroy();
	this.container.destroy();
	PureFW.ScrollBox.parent.destroy.call(this);
};
/*** AB HIER ALLER PRIVATE ***/
//	_create_scrollbox:function() {
//		var w = this.w;
//		var h = this.h;
//		if (this.vertical)
//			w -= this.scrollbar_width;
//		else
//			h -= this.scrollbar_width;
//			
//		var inner_html = 
//			'<div id="'+this.container_id+'" '+
//			' class="ScrollBox_container"'+
//			' style="width: '+w+'px; height: '+h+'px;"> '+ 
//			'  <div id="'+this.content_id+'" '+
//			'   class="ScrollBox_content"';
//		if (this.vertical)
//			inner_html += '   style="width: '+w+'px;"';
//		else
//			inner_html += '   style="height: '+h+'px;"';
//		inner_html += '>' +
//			'  </div>'+
//			'</div>';
//		if (this.vertical) {
//			inner_html += '<div id="'+this.scrollbar_id+'" '+
//				'style="height: '+h+'px; width: '+this.scrollbar_width+'px; '+
//				'float: left;">'+
//				'<div onMouseDown="javascript: ScrollBox__instances['+this.id+']._scroll_minus_start();"'+
//				' onMouseUp="javascript: ScrollBox__instances['+this.id+']._scroll_minus_stop();"'+
//				' onMouseOut="javascript: ScrollBox__instances['+this.id+']._scroll_minus_stop();"'+
//				' class="ScrollBox_top_arrow"'+
//				' style="width: '+this.scrollbar_width+'px; height: 10px"></div>'+
//				'<div style="width: '+this.scrollbar_width+'px; height: '+(h-20)+'px;"></div>'+
//				'<div onMouseDown="javascript: ScrollBox__instances['+this.id+']._scroll_plus_start();"'+
//				' onMouseUp="javascript: ScrollBox__instances['+this.id+']._scroll_plus_stop();"'+
//				' onMouseOut="javascript: ScrollBox__instances['+this.id+']._scroll_plus_stop();"'+
//				' class="ScrollBox_bottom_arrow"'+
//				' style="width: '+this.scrollbar_width+'px; height: 10px;"></div>'+
//				'</div>';
//		}
//		else {
//			inner_html += '<div id="'+this.scrollbar_id+'" '+
//				'style="width: '+w+'px; height: '+this.scrollbar_width+'px; '+
//				'clear: left;">'+
//				'<div onMouseDown="javascript: ScrollBox__instances['+this.id+']._scroll_minus_start();"'+
//				' onMouseUp="javascript: ScrollBox__instances['+this.id+']._scroll_minus_stop();"'+
//				' onMouseOut="javascript: ScrollBox__instances['+this.id+']._scroll_minus_stop();"'+
//				' class="ScrollBox_left_arrow"'+
//				' style="width: 10px; height: '+this.scrollbar_width+'px;"></div>'+
//				'<div style="width: '+(w-20)+'px; height: '+this.scrollbar_width+'px; float:left"></div>'+
//				'<div onMouseDown="javascript: ScrollBox__instances['+this.id+']._scroll_plus_start();"'+
//				' onMouseUp="javascript: ScrollBox__instances['+this.id+']._scroll_plus_stop();"'+
//				' onMouseOut="javascript: ScrollBox__instances['+this.id+']._scroll_plus_stop();"'+
//				' class="ScrollBox_right_arrow"'+
//				' style="width: 10px; height: '+this.scrollbar_width+'px;"></div>'+
//				'</div>';
//		}
//		if (this.can_DOM) {
//			var new_div = document.createElement('div');
//			new_div.id=this.scrollbox_id;
//			new_div.innerHTML = inner_html;
//			this.parent_node.appendChild(new_div);
//		}
//		else {
//			//TODO innerHTML-Variante für non-DOM (vgl. AJAXWindow::create_window)
//		}
//	},

/*
iens6=document.all||document.getElementById
ns4=document.layers

//specify speed of scroll (greater=faster)
var speed=5

if (iens6){
document.write('<div id="container" style="position:relative;width:175px;height:160px;border:1px solid black;overflow:hidden">')
document.write('<div id="content" style="position:absolute;width:170px;left:0;top:0">')
}
</script>

<ilayer name="nscontainer" width=175 height=160 clip="0,0,175,160">
<layer name="nscontent" width=175 height=160 visibility=hidden>

<!--INSERT CONTENT HERE-->
<p><font size="2" face="Arial">-</font><font size="2" face="Arial"> DHTML is the
combination of HTML, JavaScript, and CSS</font></p>
<p><font size="2" face="Arial">- DOM stands for Document Object Model</font></p>
<p><font size="2" face="Arial">-</font><font size="2" face="Arial"> DHTML allows
content on a page to change on the fly, without reloading the page</font></p>
<p><font size="2" face="Arial">- CSS allows for the separation between content
definition and formatting</font></p>
<p><font size="2" face="Arial">- CSS stands for Cascading style sheet</font></p>
<p><font size="2" face="Arial">- </font><font size="2" face="Arial"><a href="http://www.dynamicdrive.com">Dynamic
Drive</a> provides free, cut and paste DHTML scripts</font></p>
<!--END CONTENT-->

</layer>
</ilayer>

<script language="JavaScript1.2">
if (iens6)
document.write('</div></div>')
</script>

<table width="175px"><td><p align="right">
<a href="#" onMouseover="moveup()" onMouseout="clearTimeout(moveupvar)"><img src="up.gif" border=0></a>  <a href="#" onMouseover="movedown()" onMouseout="clearTimeout(movedownvar)"><img src="down.gif" border=0></a></p></td>
</table>

<script language="JavaScript1.2">
if (iens6){
var crossobj=document.getElementById? document.getElementById("content") : document.all.content
var contentheight=crossobj.offsetHeight
}
else if (ns4){
var crossobj=document.nscontainer.document.nscontent
var contentheight=crossobj.clip.height
}

function movedown(){
if (iens6&&parseInt(crossobj.style.top)>=(contentheight*(-1)+100))
crossobj.style.top=parseInt(crossobj.style.top)-speed+"px"
else if (ns4&&crossobj.top>=(contentheight*(-1)+100))
crossobj.top-=speed
movedownvar=setTimeout("movedown()",20)
}

function moveup(){
if (iens6&&parseInt(crossobj.style.top)<=0)
crossobj.style.top=parseInt(crossobj.style.top)+speed+"px"
else if (ns4&&crossobj.top<=0)
crossobj.top+=speed
moveupvar=setTimeout("moveup()",20)

}

function getcontent_height(){
if (iens6)
contentheight=crossobj.offsetHeight
else if (ns4)
document.nscontainer.document.nscontent.visibility="show"
}
window.onload=getcontent_height
</script>*/