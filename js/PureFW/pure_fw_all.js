PureFW=new Object();PureFW.Point=function(x,y){this.x=x;this.y=y;}
PureFW.instance_of=function(a,b){if(!a.constructor){return false;}
if((a.constructor===b)||((b.constructor&&(b.constructor==a.constructor)))){return true;}
if(!a.constructor.parent){return false;}
return PureFW.instance_of(a.constructor.parent,b);}
;PureFW.equals=function(ab,bb,cb){if((typeof(ab)!='object')||(typeof(bb)!='object'))return false;var n=0;for(var i in ab){if(typeof(ab[i])!=typeof(bb[i]))return false;if(typeof(ab[i])=='object'){if(!PureFW.equals(ab[i],bb[i],cb))return false;}
if((cb&&(ab[i]!==bb[i]))||(!cb&&(ab[i]!=bb[i])))return false;n++;}
var m=0;for(var i in bb)m++;if(n!=m)return false;return true;}
PureFW.copy_style=function(db,eb){for(s in db.style){try{eb.style[s]=db.style[s];}
catch(e){}
}
}
;PureFW.get_url_params=function(fb){var gb=fb.substr(1).split('&');var n=gb.length;var hb;var ib=new Object();for(var i=0;i<n;i++){hb=gb[i].split('=');ib[hb[0]]=hb[1];}
return ib;}
;PureFW.trim=function(s,jb){if(s)return PureFW.ltrim(PureFW.rtrim(s,jb),jb);else
return'';}
;PureFW.ltrim=function(s,jb){if(s)return s.replace(new RegExp("^["+(jb||"\\s")+"]+","g"),"");else
return'';}
;PureFW.rtrim=function(s,jb){if(s)return s.replace(new RegExp("["+(jb||"\\s")+"]+$","g"),"");else
return'';}
;PureFW.iframes_to_div=function(){var kb=document.getElementsByTagName('iframe');var n=kb.length;var i,j,lb,mb,m,nb;for(i=0;i<n;i++){lb=kb[i];if(typeof(lb)=='undefined')continue;PureFW.iframe_to_div(kb[i]);}
}
;PureFW.iframe_to_div=function(lb,ob){ob=ob||top;pb=ob[lb.name].document.getElementsByTagName('body')[0];qb=document.createElement('div');PureFW.copy_style(lb,qb);qb.style.width=(lb.width.indexOf('%')==-1)?lb.width+'px':lb.width;qb.style.height=(lb.height.indexOf('%')==-1)?lb.height+'px':lb.height;if(lb.getAttribute("frameborder")!=='0')qb.style.border='2px black solid';if(lb.getAttribute("scrolling")!=='no')qb.style.overflow='auto';else
qb.style.overflow='hidden';qb.id=lb.id;qb.name=lb.name;qb.innerHTML=pb.innerHTML;lb.parentNode.replaceChild(qb,lb);}
PureFW.body_loaded=function(rb){if(!document.body)document.body=document.getElementsByTagName("body")[0];if(false){PureFW.iframes_to_div();if(PureFW.WidgetManager){PureFW.WidgetManager.manager_all.set_scale_value(PureFW.WidgetManager.scale_reference_value<<1
);PureFW.WidgetManager.manager_all.set_quality('low');}
var sb=document.createElement('meta');sb.setAttribute("id","viewport");sb.setAttribute("name","viewport");sb.setAttribute("content","maximum-scale = 10");document.getElementsByTagName("head")[0].appendChild(sb);var tb=document.createElement("div");tb.style.position="absolute";tb.style.left="0";tb.style.top="0";tb.style.zIndex=1000;tb.style.width="2000px";tb.style.height="2000px";tb.style.backgroundColor="black";tb.style.display="none";tb.id="iphone_flipme";var ub=document.createElement("img");ub.src="../pix/ui/special_situations/iphone/flipme.png";ub.style.position="absolute";ub.style.left="0";ub.style.top="0";tb.appendChild(ub);document.body.appendChild(tb);document.body.onorientationchange=vb=function(){PureFW.on_orientation_change();window.setTimeout("window.scrollTo(0,1);",100);}
vb();}
else{if(PureFW.WidgetManager){var w=window.innerWidth||document.body.clientWidth;var h=window.innerHeight||document.body.clientHeight;if(PureFW.WidgetManager.page_aspect_ratio&&(h<(w/PureFW.WidgetManager.page_aspect_ratio)))w=h*PureFW.WidgetManager.page_aspect_ratio;PureFW.WidgetManager.manager_all.set_scale_value(w);PureFW.WidgetManager.manager_all.set_quality('high');}
}
}
;PureFW.handle_resize=function(rb){if((PureFW.WidgetManager)&&navigator.userAgent.toLowerCase().indexOf('iphone')<0){var w=window.innerWidth||document.body.clientWidth;var h=window.innerHeight||document.body.clientHeight;if(PureFW.WidgetManager.page_aspect_ratio&&(h<(w/PureFW.WidgetManager.page_aspect_ratio)))w=h*PureFW.WidgetManager.page_aspect_ratio;PureFW.WidgetManager.manager_all.set_scale_value(w);}
try{rb=PureFW.EventUtil.formatEvent(rb);}
catch(e){}
rb.preventDefault();}
;PureFW.on_orientation_change=function(){}
;PureFW._get_elem_by_id_frames=function(wb){var xb,i;for(i=0;i<frames.length;i++){try{xb=frames[i].document.getElementById(wb);if((xb)&&(typeof(xb)!=='undefined'))return xb;}
catch(e){}
}
return null;}
document.getElemById=function(wb){var xb;try{xb=document.getElementById(wb);if((xb)&&(typeof(xb)!=='undefined'))return xb;else{if(xb=PureFW._get_elem_by_id_frames(wb))return xb;else
return null;}
}
catch(e){if(xb=PureFW._get_elem_by_id_frames(wb))return xb;else
throw new Error("getElem("+wb+") has no properties.");}
}
;document.setElemInnerHTML=function(xb,text,yb){if(typeof(xb)==='string')xb=document.getElemById(xb);if(yb){xb.innerHTML='';var zb,$b=text.split('<script');var i,j,k,m,n=$b.length;for(i=0,k=false;i<n;i++){zb=$b[i].split('</script>');m=zb.length;for(j=0;j<m;j++,k=!k){if(k){eval(zb[j].substr(zb[j].indexOf('>')+1));}
else{xb.innerHTML=xb.innerHTML+zb[j];}
}
}
}
else
xb.innerHTML=text;}
;Function.prototype.extend=function(_b){if(_b.constructor==Function){this.prototype=new _b;this.prototype.constructor=this;this.parent=_b.prototype;}
else{for(x in _b)this.prototype[x]=_b[x];this.prototype.constructor=this;this.parent=_b;}
}
;Array.prototype.contains=function(ac,bc){if(typeof(cb)=='undefined')cb=true;var cc=false;for(var i=0;i<this.length;i++){if((cb&&(this[i]===ac))||(!cb&&this[i]==ac)){cc=true;break;}
}
return cc;}
;Array.prototype.search=function(ac,cb){if(typeof(cb)=='undefined')cb=true;if(typeof(ac)=='object'){for(var i=0;i<this.length;i++){if(PureFW.equals(this[i],ac,cb))return i;}
}
else{for(var i=0;i<this.length;i++){if((cb&&(this[i]===ac))||(!cb&&(this[i]==ac)))return i;}
}
return-1;}
Array.prototype.remove=function(ac,dc){var i=0;var n=this.length;var r=0;while((i<n)&&(!dc||((r<dc)))){if(this[i]===ac){this.splice(i,1);r++;n--;}
else{i++;}
}
return this;}
;Array.prototype.destroy=function(){for(var i=0;i<this.length;i++){if(typeof(this[i])!=='function'){try{this[i].destroy();}
catch(e){try{delete this[i];}
catch(e){}
}
}
}
try{delete this;}
catch(e){}
}
Array.prototype.shuffle=function(){var i=this.length;if(i==0)return this;while(--i){var j=Math.floor(Math.random()*(i+1));var ec=this[i];var fc=this[j];this[i]=fc;this[j]=ec;}
return this;}
Array.prototype.random=function(){return this[Math.floor(Math.random()*this.length)];}
if(!Array.prototype.push){Array.prototype.push=function(xb){this[this.length]=xb;}
}
;isArray=function(a){return((a&&(typeof a=='object'))||(typeof a=='function'))&&(typeof(a.length)!='undefined');}
if(!Math.sign){Math.sign=function(n){if(n>0)return 1;if(n<0)return-1;else
return 0;}
}
String.prototype.ucfirst=function(){return this.charAt(0).toUpperCase()+this.substr(1);}
document.getScrollX=function(){return document.documentElement&&document.documentElement.scrollLeft?document.documentElement.scrollLeft:document.body.scrollLeft;}
document.getScrollY=function(){return document.documentElement&&document.documentElement.scrollTop?document.documentElement.scrollTop:document.body.scrollTop
}
PureFW.Timeout=new Object();PureFW.Timeout.global_clock=60;PureFW.Timeout.clock_num=0;PureFW.Timeout.is_running=false;PureFW.Timeout.registered_functions_once=new Object();PureFW.Timeout.registered_functions_interval=new Object();PureFW.Timeout._tick=function(){var t=PureFW.Timeout.clock_num;PureFW.Timeout.clock_num++;window.setTimeout(PureFW.Timeout._tick,PureFW.Timeout.global_clock);if(PureFW.Timeout.registered_functions_once[t]){var n=PureFW.Timeout.registered_functions_once[t].length;try{for(var i=0;i<n;i++){if(PureFW.Timeout.registered_functions_once[t][i])PureFW.Timeout.registered_functions_once[t][i]();PureFW.Timeout.registered_functions_once[t][i]=null;}
}
catch(e){throw PureFW.Timeout.registered_functions_once[t][i]+e;}
finally{PureFW.Timeout.registered_functions_once[t]=null;}
}
for(var T in PureFW.Timeout.registered_functions_interval){if((t%T)==0){var n=PureFW.Timeout.registered_functions_interval[T].length;for(var i=0;i<n;i++){if(PureFW.Timeout.registered_functions_interval[T][i])PureFW.Timeout.registered_functions_interval[T][i]();}
}
}
}
PureFW.Timeout.start=function(){if(!PureFW.Timeout.is_running)window.setTimeout(PureFW.Timeout._tick,PureFW.Timeout.global_clock);PureFW.Timeout.is_running=true;}
PureFW.Timeout.set_global_clock=function(d){d=parseInt(d);if(gc<=0)throw new Error("Timeout::set_gobal_clock: First argument 'd' "+"has to be greater than 0");PureFW.Timeout.global_clock=d;}
;PureFW.Timeout.get_global_clock=function(){return PureFW.Timeout.global_clock;}
;PureFW.Timeout.set_timeout=function(hc,gc){if(!PureFW.Timeout.is_running)PureFW.Timeout.start();if(typeof(hc)!=='function'){throw new Error("Timeout::set_timeout: First argument 'fn' "+"has to be of type Function");}
if(isNaN(gc))throw new Error("Timeout::set_timeout: Second argument 'delay' "+"has to be of type Number");if(gc<=0)throw new Error("Timeout::set_timeout: Second argument 'delay' "+"has to be greater than 0");var t=PureFW.Timeout.clock_num
+Math.round(gc/PureFW.Timeout.global_clock);if(t==PureFW.Timeout.clock_num){hc();}
else{if((typeof(PureFW.Timeout.registered_functions_once[t])=='undefined')||!PureFW.Timeout.registered_functions_once[t]){PureFW.Timeout.registered_functions_once[t]=new Array();}
PureFW.Timeout.registered_functions_once[t].push(hc);return[t,PureFW.Timeout.registered_functions_once[t].length-1];}
}
;PureFW.Timeout.clear_timeout=function(o){if((typeof(o)=='undefined')||!o||(typeof(o)!='object')||(o.length!=2)){throw new Error("PureFW.Timeout.clear_timeout: First argument 'o' has to be "+"an Array of lenght 2");}
if(!PureFW.Timeout.registered_functions_once[o[0]]||!PureFW.Timeout.registered_functions_once[o[0]][o[1]]){return;}
PureFW.Timeout.registered_functions_once[o[0]][o[1]]=null;}
PureFW.Timeout.set_interval=function(hc,gc){if(!PureFW.Timeout.is_running)PureFW.Timeout.start();if(typeof(hc)!=='function')throw new Error("Timeout::set_interval: First argument 'fn' "+"has to be of type Function");if(isNaN(gc))throw new Error("Timeout::set_interval: Second argument 'delay' "+"has to be of type Number");if(gc<=0)throw new Error("Timeout::set_interval: Second argument 'delay' "+"has to be greater than 0");var t=Math.round(gc/PureFW.Timeout.global_clock);if(t<1)t=1;if((typeof(PureFW.Timeout.registered_functions_interval[t])=='undefined')||!PureFW.Timeout.registered_functions_interval[t]){PureFW.Timeout.registered_functions_interval[t]=new Array();}
PureFW.Timeout.registered_functions_interval[t].push(hc);return[t,PureFW.Timeout.registered_functions_interval[t].length-1];}
;PureFW.Timeout.clear_interval=function(o){if((typeof(o)=='undefined')||!o||(typeof(o)!='object')||(o.length!=2)){throw new Error("PureFW.Timeout.clear_interval: First argument 'o' has to be "+"an Array of lenght 2");}
if(!PureFW.Timeout.registered_functions_interval[o[0]]||!PureFW.Timeout.registered_functions_interval[o[0]][o[1]]){return;}
PureFW.Timeout.registered_functions_interval[o[0]][o[1]]=null;}
PureFW.Time=new Object();PureFW.Time.LNG_DAY='day';PureFW.Time.LNG_DAYS='days';var ic=new Date();PureFW.Time.init_client_time=Math.floor(ic.getTime()/1000);PureFW.Time.set_init_server_time=function(jc){PureFW.Time.init_server_time=jc;}
PureFW.Time.set_init_client_time=function(jc){PureFW.Time.init_client_time=jc;}
PureFW.Time.get_current_client_time=function(){var ic=new Date();return Math.floor(ic.getTime()/1000);}
PureFW.Time.get_current_server_time=function(){return PureFW.Time.get_current_client_time()+PureFW.Time.get_client_server_diff();}
PureFW.Time.get_client_server_diff=function(){return PureFW.Time.init_server_time-PureFW.Time.init_client_time;}
PureFW.Time.sec_in_time=function(kc,lc,mc){var nc=0;var oc=kc;if(typeof(lc)=='undefined')lc=true;if(typeof(mc)=='undefined')mc=true;while(oc>=86400){oc=oc-86400;nc=nc+1;}
pc=new Date(0);var qc=pc.getTimezoneOffset();var t=(oc+qc*60)*1000;date=new Date(t);if(lc){if(mc){rc=date.formatDate('H:i:s');}
else{rc=date.formatDate('G:i:s');}
}
else{if(mc){rc=date.formatDate("H:i");}
else{rc=date.formatDate("G:i");}
}
if(nc!=0){if(nc==1)return(nc+" "+PureFW.Time.LNG_DAY+", "+rc);else
return(nc+" "+PureFW.Time.LNG_DAYS+", "+rc);}
else{return rc;}
}
PureFW.EventUtil=new Object();PureFW.EventUtil.add_event_handler=function(sc,tc,hc){if(sc.addEventListener)sc.addEventListener(tc,hc,false);else if(sc.attachEvent)sc.attachEvent("on"+tc,hc);}
;PureFW.EventUtil.remove_event_handler=function(sc,tc,hc){if(sc.removeEventListener)sc.removeEventListener(tc,hc,false);else if(sc.detachEvent)sc.detachEvent("on"+tc,hc);}
;PureFW.EventUtil.formatEvent=function(rb){if(!rb)rb=window.event;if(!rb.stopPropagation){rb.charCode=(rb.type=="keypress")?rb.keyCode:0;rb.eventPhase=2;rb.isChar=(rb.charCode>0);rb.pageX=rb.clientX+document.body.scrollLeft;rb.pageY=rb.clientY+document.body.scrollTop;rb.preventDefault=function(){this.returnValue=false;}
;if(rb.type=="mouseout"){rb.relatedTarget=rb.toElement;}
else if(rb.type=="mouseover"){rb.relatedTarget=rb.fromElement;}
rb.stopPropagation=function(){this.cancelBubble=true;}
;rb.target=rb.srcElement;rb.time=(new Date).getTime();}
return rb;}
;PureFW.EventUtil.create_event=function(tc,uc){var rb=new Object();rb.bubbles=false;rb.cancelable=false;rb.target=null;rb.timeStamp=(new Date()).getTime();rb.type=tc;rb.detail=uc||0;rb.preventDefault=rb.stopPropagation=function(){}
;return rb;}
PureFW.MouseFeatures=new Object();PureFW.MouseFeatures.get_cursor_pos=function(){return PureFW.MouseFeatures.__cursor_pos;}
PureFW.MouseFeatures.__cursor_pos=new PureFW.Point(0,0);PureFW.MouseFeatures.__update_cursor_pos_by_event=function(rb){try{rb=PureFW.EventUtil.formatEvent(rb);}
catch(e){}
try{if(rb.clientX&&rb.clientY){PureFW.MouseFeatures.__cursor_pos.x=rb.clientX
+document.getScrollX();PureFW.MouseFeatures.__cursor_pos.y=rb.clientY
+document.getScrollY();}
else if(rb.pageX&&rb.pageY){PureFW.MouseFeatures.__cursor_pos.x=rb.pageX;PureFW.MouseFeatures.__cursor_pos.y=rb.pageY;}
else{PureFW.MouseFeatures.__cursor_pos.x=0;PureFW.MouseFeatures.__cursor_pos.y=0;}
}
catch(e){}
}
PureFW.MouseFeatures.Dragging=new Object();PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_NONE=0;PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X=1;PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_Y=2;PureFW.MouseFeatures.Dragging.__cur_node=null;PureFW.MouseFeatures.Dragging.__restricted_direction=PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_NONE;PureFW.MouseFeatures.Dragging.__start_cursor_pos=new PureFW.Point(0,0);PureFW.MouseFeatures.Dragging.__start_obj_pos=new PureFW.Point(0,0);PureFW.MouseFeatures.Dragging.__max_pos=new PureFW.Point(false,false);PureFW.MouseFeatures.Dragging.__min_pos=new PureFW.Point(false,false);PureFW.MouseFeatures.Dragging.__on_dragging_functions=new Array();PureFW.MouseFeatures.Dragging.__on_drop_functions=new Array();PureFW.MouseFeatures.Dragging.add_event_handler=function(tc,hc){if(typeof(tc)!=='string')throw new Error("Widget::remove_event_handler: First argument 'type' "+"has to be of type 'string'");if(typeof(hc)!=='function')throw new Error("Widget::remove_event_handler: Second argument 'fn' "+"has to be of type 'function'");if(tc=="dragging")PureFW.MouseFeatures.Dragging.__on_dragging_functions.push(hc);else if(tc=="drop"){PureFW.MouseFeatures.Dragging.__on_drop_functions.push(hc);}
}
PureFW.MouseFeatures.Dragging.remove_event_handler=function(tc,hc){if(typeof(tc)!=='string')throw new Error("Widget::remove_event_handler: First argument 'type' "+"has to be of type 'string'");if(typeof(hc)=='undefined'){if(tc=="dragging"){PureFW.MouseFeatures.Dragging.__on_dragging_functions.destroy();PureFW.MouseFeatures.Dragging.__on_dragging_functions=null;PureFW.MouseFeatures.Dragging.__on_dragging_functions=new Array();}
else{PureFW.MouseFeatures.Dragging.__on_drop_functions.destroy();PureFW.MouseFeatures.Dragging.__on_drop_functions=null;PureFW.MouseFeatures.Dragging.__on_drop_functions=new Array();}
}
else{if(typeof(hc)!=='function')throw new Error("Widget::remove_event_handler: Second argument 'fn' "+"has to be of type 'function'");if(tc=="dragging")PureFW.MouseFeatures.Dragging.__on_dragging_functions.remove(hc);else if(tc=="drop")PureFW.MouseFeatures.Dragging.__on_drop_functions.remove(hc);}
}
PureFW.MouseFeatures.Dragging.on_dragging=function(rb){if(!rb)rb=PureFW.EventUtil.create_event("dragging");var n=PureFW.MouseFeatures.Dragging.__on_dragging_functions.length;for(var i=0;i<n;i++){PureFW.MouseFeatures.Dragging.__on_dragging_functions[i].call(PureFW.MouseFeatures.Dragging.__cur_node,rb);}
}
PureFW.MouseFeatures.Dragging.on_drop=function(rb){if(!rb)rb=PureFW.EventUtil.create_event("drop");var n=PureFW.MouseFeatures.Dragging.__on_drop_functions.length;for(var i=0;i<n;i++){PureFW.MouseFeatures.Dragging.__on_drop_functions[i].call(PureFW.MouseFeatures.Dragging.__cur_node,rb);}
}
PureFW.MouseFeatures.Dragging.start_dragging=function(vc){PureFW.MouseFeatures.Dragging.stop_dragging();if(PureFW.instance_of(vc,PureFW.Widget)){vc=vc.get_node();}
PureFW.MouseFeatures.Dragging.__cur_node=vc;PureFW.MouseFeatures.Dragging.__start_cursor_pos.x
=PureFW.MouseFeatures.__cursor_pos.x;PureFW.MouseFeatures.Dragging.__start_cursor_pos.y
=PureFW.MouseFeatures.__cursor_pos.y;PureFW.MouseFeatures.Dragging.__start_obj_pos.x
=parseInt(vc.style.left);PureFW.MouseFeatures.Dragging.__start_obj_pos.y
=parseInt(vc.style.top);}
PureFW.MouseFeatures.Dragging.stop_dragging=function(){PureFW.MouseFeatures.Dragging.__max_pos.x
=PureFW.MouseFeatures.Dragging.__max_pos.y
=PureFW.MouseFeatures.Dragging.__min_pos.x
=PureFW.MouseFeatures.Dragging.__min_pos.y=false;PureFW.MouseFeatures.Dragging.__restricted_direction=PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_NONE;if(PureFW.MouseFeatures.Dragging.__cur_node){PureFW.MouseFeatures.Dragging.on_drop(PureFW.EventUtil.create_event("drop",new PureFW.Point(parseInt(PureFW.MouseFeatures.Dragging.__cur_node.style.left),parseInt(PureFW.MouseFeatures.Dragging.__cur_node.style.top))));PureFW.MouseFeatures.Dragging.__cur_node=null
}
PureFW.MouseFeatures.Dragging.remove_event_handler("dragging");PureFW.MouseFeatures.Dragging.remove_event_handler("drop");}
PureFW.MouseFeatures.Dragging.restrict_direction=function(wc){if((wc<0)||(wc>2))throw new Error("dir has to be in {0, 1, 2}, '"+wc+"' given.");PureFW.MouseFeatures.Dragging.__restricted_direction=wc;}
PureFW.MouseFeatures.Dragging.set_pos_limits=function(xc,yc,zc,$c){if(typeof(zc)=='undefined')zc=false;if(typeof($c)=='undefined')$c=false;if(typeof(xc)=='undefined')xc=false;if(typeof(yc)=='undefined')yc=false;PureFW.MouseFeatures.Dragging.__max_pos.x=zc;PureFW.MouseFeatures.Dragging.__max_pos.y=$c;PureFW.MouseFeatures.Dragging.__min_pos.x=xc;PureFW.MouseFeatures.Dragging.__min_pos.y=yc;}
PureFW.MouseFeatures.on_mouse_move=function(rb){try{rb=PureFW.EventUtil.formatEvent(rb);}
catch(e){}
PureFW.MouseFeatures.__update_cursor_pos_by_event(rb);if(PureFW.MouseFeatures.Dragging.__cur_node){var _c=new PureFW.Point();if(PureFW.MouseFeatures.Dragging.__restricted_direction
!=PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_Y){_c.x=PureFW.MouseFeatures.Dragging.__start_obj_pos.x
+PureFW.MouseFeatures.__cursor_pos.x
-PureFW.MouseFeatures.Dragging.__start_cursor_pos.x;if((PureFW.MouseFeatures.Dragging.__max_pos.x!==false)&&PureFW.MouseFeatures.Dragging.__max_pos.x<_c.x){_c.x=PureFW.MouseFeatures.Dragging.__max_pos.x;}
else if((PureFW.MouseFeatures.Dragging.__min_pos.x!==false)&&PureFW.MouseFeatures.Dragging.__min_pos.x>_c.x){_c.x=PureFW.MouseFeatures.Dragging.__min_pos.x;}
PureFW.MouseFeatures.Dragging.__cur_node.style.left=_c.x+'px';}
else
_c.x=PureFW.MouseFeatures.Dragging.__start_obj_pos.x;if(PureFW.MouseFeatures.Dragging.__restricted_direction
!=PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X){_c.y=PureFW.MouseFeatures.Dragging.__start_obj_pos.y
+PureFW.MouseFeatures.__cursor_pos.y
-PureFW.MouseFeatures.Dragging.__start_cursor_pos.y;if((PureFW.MouseFeatures.Dragging.__max_pos.y!==false)&&PureFW.MouseFeatures.Dragging.__max_pos.y<_c.y){_c.y=PureFW.MouseFeatures.Dragging.__max_pos.y;}
else if((PureFW.MouseFeatures.Dragging.__min_pos.y!==false)&&PureFW.MouseFeatures.Dragging.__min_pos.y>_c.y){_c.y=PureFW.MouseFeatures.Dragging.__min_pos.y;}
PureFW.MouseFeatures.Dragging.__cur_node.style.top=_c.y+'px';}
else
_c.y=PureFW.MouseFeatures.Dragging.__start_obj_pos.y;PureFW.MouseFeatures.Dragging.on_dragging(PureFW.EventUtil.create_event("dragging",_c));}
}
PureFW.EventUtil.add_event_handler(document,"mousemove",PureFW.MouseFeatures.on_mouse_move);PureFW.MouseFeatures.on_mouse_up=function(rb){PureFW.MouseFeatures.Dragging.stop_dragging();}
PureFW.EventUtil.add_event_handler(document,"mouseup",PureFW.MouseFeatures.on_mouse_up);PureFW.MouseFeatures.WheelScrolling=new Object();PureFW.MouseFeatures.WheelScrolling.__cur_node=null;PureFW.MouseFeatures.WheelScrolling.__max_pos=new PureFW.Point(false,false);PureFW.MouseFeatures.WheelScrolling.__min_pos=new PureFW.Point(false,false);PureFW.MouseFeatures.WheelScrolling.__vertical_scrolling=true;PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions=new Array();PureFW.MouseFeatures.WheelScrolling.start_scrolling=function(vc,ad){PureFW.MouseFeatures.WheelScrolling.stop_scrolling();if(PureFW.instance_of(vc,PureFW.Widget)){vc=vc.get_node();}
PureFW.MouseFeatures.WheelScrolling.__cur_node=vc;if(typeof(ad)=='undefined')ad=true;PureFW.MouseFeatures.WheelScrolling.__vertical_scrolling=ad;}
PureFW.MouseFeatures.WheelScrolling.stop_scrolling=function(){PureFW.MouseFeatures.WheelScrolling.__vertical_scrolling=true;PureFW.MouseFeatures.WheelScrolling.__min_pos.x
=PureFW.MouseFeatures.WheelScrolling.__min_pos.y
=PureFW.MouseFeatures.WheelScrolling.__max_pos.x
=PureFW.MouseFeatures.WheelScrolling.__max_pos.y=false;PureFW.MouseFeatures.WheelScrolling.__cur_node=null;}
PureFW.MouseFeatures.WheelScrolling.add_event_handler=function(tc,hc){if(typeof(tc)!=='string')throw new Error("Widget::remove_event_handler: First argument 'type' "+"has to be of type 'string'");if(typeof(hc)!=='function')throw new Error("Widget::remove_event_handler: Second argument 'fn' "+"has to be of type 'function'");if(tc=="scroll")PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.push(hc);}
PureFW.MouseFeatures.WheelScrolling.remove_event_handler=function(tc,hc){if(typeof(tc)!=='string')throw new Error("Widget::remove_event_handler: First argument 'type' "+"has to be of type 'string'");if(typeof(hc)=='undefined'){if(tc=="scroll"){PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.destroy();PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions=null;PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions
=new Array();}
}
else{if(typeof(hc)!=='function')throw new Error("Widget::remove_event_handler: Second argument 'fn' "+"has to be of type 'function'");if(tc=="scroll")PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.remove(hc);}
}
PureFW.MouseFeatures.WheelScrolling.on_scroll=function(rb){if(!rb)rb=PureFW.EventUtil.create_event("scroll");var n=PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.length;for(var i=0;i<n;i++){PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions[i].call(PureFW.MouseFeatures.WheelScrolling.__cur_node,rb);}
}
PureFW.MouseFeatures.WheelScrolling.__scroll=function(rb){if(PureFW.MouseFeatures.WheelScrolling.__cur_node){var bd=0;try
{event=PureFW.EventUtil.formatEvent(event);}
catch(e){}
if(event.wheelDelta){bd=event.wheelDelta/120;if(window.opera)bd=-bd;}
else if(event.detail){bd=-event.detail/3;}
event.preventDefault();PureFW.MouseFeatures.WheelScrolling.on_scroll(PureFW.EventUtil.create_event("scroll",bd))}
}
PureFW.PicPath=new Object();PureFW.PicPath.reference_path='../pix/';PureFW.PicPath.current_path=PureFW.PicPath.reference_path;PureFW.WidgetManager=function(cd){this.initial_font_size=cd||13;this.scale_factor=1;}
PureFW.WidgetManager.scale_reference_value=1000;PureFW.WidgetManager.page_aspect_ratio=0;PureFW.WidgetManager.prototype.registered_widgets=new Array();PureFW.WidgetManager.prototype.register_widget=function(dd){if(!dd.instance_of(PureFW.Widget)){throw new Error("First argument has to be a Widget-Object in "+"WidgetScalar::register_widget"+arguments.callee.caller);}
this.registered_widgets.push(dd);}
;PureFW.WidgetManager.prototype.unregister_widget=function(dd){if(!dd.instance_of(PureFW.Widget)){throw new Error("First argument has to be a Widget-Object in "+"WidgetScalar::unregister_widget");}
this.registered_widgets.remove(dd);}
;PureFW.WidgetManager.prototype.set_quality=function(ed){var i,n=this.registered_widgets.length;for(i=0;i<n;i++){if(this.registered_widgets[i]){this.registered_widgets[i].set_quality(ed);}
}
}
;PureFW.WidgetManager.prototype.set_scale_value=function(fd){this.scale(fd/this.constructor.scale_reference_value);}
;PureFW.WidgetManager.prototype.scale=function(gd){if(gd==this.scale_factor)return;this.scale_factor=gd;var i,n=this.registered_widgets.length;for(i=0;i<n;i++){if(this.registered_widgets[i]){this.registered_widgets[i].scale(gd);}
}
document.body.style.fontSize=(this.initial_font_size*gd)+"px";}
;PureFW.WidgetManager.prototype.refresh_fonts=function(){document.body.style.fontSize=(this.initial_font_size*this.scale_factor)+"px";}
PureFW.WidgetManager.prototype.get_scale_factor=function(){return this.scale_factor;}
;PureFW.WidgetManager.prototype.set_pic_path=function(hd){var i,n=this.registered_widgets.length;for(i=0;i<n;i++){if(this.registered_widgets[i]){this.registered_widgets[i].set_pic_path(hd);}
}
}
;PureFW.WidgetManager.prototype.repaint=function(){var i,n=this.registered_widgets.length;for(i=0;i<n;i++){if(this.registered_widgets[i]){this.registered_widgets[i].repaint();}
}
}
;PureFW.WidgetManager.prototype.insert_into_dom_tree=function(){var i,n=this.registered_widgets.length;for(i=0;i<n;i++){if(this.registered_widgets[i]){this.registered_widgets[i].insert_into_dom_tree();}
}
}
;PureFW.WidgetManager.manager_all=new PureFW.WidgetManager();PureFW.AJAXClientServer=new Object();PureFW.AJAXClientServer.callback_functions=new Array();PureFW.AJAXClientServer.session_name='';PureFW.AJAXClientServer.session_id='';PureFW.AJAXClientServer.xml_http_requests=new Array();PureFW.AJAXClientServer.xhr_available=new Array();PureFW.AJAXClientServer.request_num=0;PureFW.AJAXClientServer.CMD_RELOGIN=1;PureFW.AJAXClientServer.VALUE=100;PureFW.AJAXClientServer.VALUE_SIMPLE=101;PureFW.AJAXClientServer.SESSION=103;PureFW.AJAXClientServer.EXCEPTION=104;PureFW.AJAXClientServer.create_new_request=function(){if(typeof XMLHttpRequest!='undefined')return new XMLHttpRequest();else if(window.ActiveXObject){var id=["Microsoft.XmlHttp","MSXML2.XmlHttp","MSXML2.XmlHttp.3.0","MSXML2.XmlHttp.4.0","MSXML2.XmlHttp.5.0"];for(var i=id.length-1;i>=0;i--){try{jd=new ActiveXObject(id[i]);return jd;}
catch(e){}
}
}
throw new Error('XMLHttp (AJAX) not supported');}
;PureFW.AJAXClientServer.get_xhr_index=function(){var n=PureFW.AJAXClientServer.xhr_available.length;var ib;for(var i=0;i<n;i++){if(PureFW.AJAXClientServer.xhr_available[i]){ib=i;break;}
}
if(i==n){PureFW.AJAXClientServer.xml_http_requests[n]=PureFW.AJAXClientServer.create_new_request();ib=n;}
PureFW.AJAXClientServer.xhr_available[ib]=false;return ib;}
;PureFW.AJAXClientServer.send_request=function(kd,ld,md,nd){var od=PureFW.AJAXClientServer.get_xhr_index();var pd=PureFW.AJAXClientServer.xml_http_requests[od];PureFW.AJAXClientServer.callback_functions[od]=ld;var qd=(typeof(md)=='undefined')?"GET":"POST";kd=(kd.indexOf('?')==-1)?kd+'?':kd+'&';kd+="anti_cache="+PureFW.AJAXClientServer.request_num;if(kd.indexOf('&s=')==-1){kd+="&"+PureFW.AJAXClientServer.session_name+"="+PureFW.AJAXClientServer.session_id;}
pd.open(qd,kd,true);pd.onreadystatechange=function(){var rd=PureFW.AJAXClientServer.xml_http_requests[od];if((rd.readyState==4)&&(rd.status==200)){PureFW.AJAXClientServer.xhr_available[od]=true;if(PureFW.AJAXClientServer.callback_functions[od]){if(nd){PureFW.AJAXClientServer.callback_functions[od].call(this,rd.responseText);}
else
{PureFW.AJAXClientServer.callback_functions[od].call(this,PureFW.AJAXClientServer.parse(rd.responseText));}
}
;}
}
;if(qd=="POST"){pd.setRequestHeader("Content-type","application/x-www-form-urlencoded;charset=UTF-8");pd.send(encodeURI(md));}
else{pd.setRequestHeader("Content-type","text/plain;charset=UTF-8");pd.send(null);}
PureFW.AJAXClientServer.request_num++;return true;}
;PureFW.AJAXClientServer.show_reload_demand_box=null;PureFW.AJAXClientServer.show_reload_demand=function(){if(PureFW.AJAXClientServer.show_reload_demand_box)return;var sd=PureFW.WidgetManager.scale_reference_value;var td=PureFW.WidgetManager.scale_reference_value/PureFW.WidgetManager.page_aspect_ratio;var ud=new PureFW.ConfirmationBox(document.body,(sd-350)>>1,(td-220)>>1,350,220,PureFW.ConfirmationBox.NO_BUTTONS,true
);ud.set_bg_color('#ecc');ud.set_font_color('#000');ud.set_content('The session has timed out or a server error has occured. The page has '+'to be reloaded in order to continue.<br/>'+'<br/>'+'Unfortunately, the security settings of your browser do not allow '+'an automated page reload.<br/>'+'<br/>'+'<b>Please reload the page manually by pressing F5</b>.');PureFW.AJAXClientServer.show_reload_demand_box=ud;}
PureFW.AJAXClientServer.parse=function(vd){var wd=vd.split("||");var ib=new Array();while(wd.length>0){var t=wd.shift();switch(parseInt(t)){case PureFW.AJAXClientServer.CMD_RELOGIN:try{top.location.reload();}
catch(e){PureFW.AJAXClientServer.show_reload_demand();}
return;case PureFW.AJAXClientServer.VALUE:case PureFW.AJAXClientServer.VALUE_SIMPLE:ib.push(wd.shift());break;case PureFW.AJAXClientServer.SESSION:PureFW.AJAXClientServer.session_name=wd.shift();PureFW.AJAXClientServer.session_id=wd.shift();break;case PureFW.AJAXClientServer.EXCEPTION:wd.shift();throw new Error(wd.shift());default:if(vd.indexOf('logout()')!=-1){try{top.location.reload();}
catch(e){PureFW.AJAXClientServer.show_reload_demand();}
return;}
else
throw new Error("PARSING_ERROR: "+vd);}
}
return ib;}
;PureFW.Widget=new Object();PureFW.Widget.id=null;PureFW.Widget.position=new PureFW.Point(0,0);PureFW.Widget.width=0;PureFW.Widget.height=0;PureFW.Widget.parent_node=null;PureFW.Widget.z_index=1;PureFW.Widget.pic_path='';PureFW.Widget.positioning='absolute';PureFW.Widget.floating='none';PureFW.Widget.node=null;PureFW.Widget.init=function(xd,x,y,w,h,yd){this.no_scale=yd||false;this.width=parseInt(w)||0;this.height=parseInt(h)||0;this.position=new PureFW.Point(parseInt(x)||0,parseInt(y)||0);if(xd){if(PureFW.instance_of(xd,PureFW.Container)){xd=xd.get_content_node();}
else if(PureFW.instance_of(xd,PureFW.Widget)){xd=xd.get_node();}
if(typeof(xd.appendChild)=='undefined'){throw Error("Wrong parent_node given!");}
}
this.parent_node=xd||document.body;this.activated=true;this.opacity=1;this._active_opacity=this.opacity=1;this.current_fading_interval=0;this.active_tweens=new Object();this.on_destroy_functions=new Array();this.on_click_functions=new Array();this.on_hide_functions=new Array();this.on_show_functions=new Array();this.on_mouseover_functions=new Array();this.on_mouseout_functions=new Array();this.on_mousedown_functions=new Array();this.on_mouseup_functions=new Array();this.on_fade_out_begin_functions=new Array();this.on_fade_out_end_functions=new Array();this.on_fade_in_begin_functions=new Array();this.on_fade_in_end_functions=new Array();this.quality='high';if(this.no_scale)this.scale_factor=1;else
this.scale_factor=PureFW.WidgetManager.manager_all.get_scale_factor();if(this.scale_factor==1)this.pic_path=PureFW.PicPath.reference_path;else
this.pic_path=PureFW.PicPath.reference_path+'scales/'+this.scale_factor+'/';}
;PureFW.Widget.get_position=function(){return new PureFW.Point(this.get_x(),this.get_y());}
;PureFW.Widget.get_x=function(){return Math.round(this.position.x*this.scale_factor);}
;PureFW.Widget.get_y=function(){return Math.round(this.position.y*this.scale_factor);}
;PureFW.Widget.set_x=function(x,zd,$d,_d){x=parseInt(x);xb=this.get_node();var ae=null;if(xb){if(!zd){xb.style.left=x*this.scale_factor+'px';}
else{if(!this.active_tweens)this.active_tweens=new Object();if(this.active_tweens.set_x)this.active_tweens.set_x.stop();ae=new Tween(xb.style,'left',$d,this.get_x(),x*this.scale_factor,zd/1000,'px');this.active_tweens.set_x=ae;ae.onMotionFinished=(function(be,ce,de){return function(){be.active_tweens.set_x=null;if(de)de.call(this);}
}
)(this,x,_d);ae.onMotionStopped=(function(be){return function(ee){be.position.x
=ee.target._pos/be.scale_factor;be.active_tweens.set_x=null;}
}
)(this);ae.start();}
}
this.position.x=x;return ae;}
;PureFW.Widget.set_y=function(y,zd,$d,_d){y=parseInt(y);xb=this.get_node();var ae=null;if(xb){if(!zd){xb.style.top=y*this.scale_factor+'px';}
else{if(!this.active_tweens)this.active_tweens=new Object();if(this.active_tweens.set_y)this.active_tweens.set_y.stop();ae=new Tween(xb.style,'top',$d,this.get_y(),y*this.scale_factor,zd/1000,'px');this.active_tweens.set_y=ae;ae.onMotionFinished=(function(be,fe,de){return function(){be.active_tweens.set_y=null;if(de)de.call(this);}
}
)(this,y,_d);ae.onMotionStopped=(function(be){return function(ee){be.position.y=ee.target._pos/be.scale_factor;be.active_tweens.set_y=null;}
}
)(this);ae.start();}
}
this.position.y=y;return ae;}
;PureFW.Widget.set_position=function(x,y,zd,$d,_d){delete this.position;x=parseInt(x);y=parseInt(y);if((typeof(x)=='object')&&(x.constructor==PureFW.Point)){x=x.x;y=x.y;zd=y;$d=zd;}
else if(typeof(x)=='object')throw new Error("PureFW.Widget::set_position: "+"First argument is an object, but no PureFW.Point");if(x==this.get_x())x=this.position.x;if(y==this.get_y())y=this.position.y;var xb=this.get_node();if(xb){if(!zd){xb.style.left=x*this.scale_factor+'px';xb.style.top=y*this.scale_factor+'px';}
else{this.set_x(x,zd,$d,_d);this.set_y(y,zd,$d);}
}
this.position=new PureFW.Point(x,y);}
;PureFW.Widget.get_width=function(){return Math.round(this.width*this.scale_factor);}
;PureFW.Widget.set_width=function(w,zd,$d,_d){w=parseInt(w);var xb=this.get_node();var ae=null;if(!xb){this.width=w;}
else{if(!zd){this.width=w;this.repaint();}
else{if(!this.active_tweens)this.active_tweens=new Object();if(this.active_tweens.set_width)this.active_tweens.set_width.stop();ae=new Tween(xb.style,'width',$d,this.get_width(),w*this.scale_factor,zd/1000,'px');this.active_tweens.set_width=ae;ae.onMotionFinished=(function(be,ge,de){return function(){be.active_tweens.set_width=null;be.width=ge;be.repaint();if(de)de.call(this);}
}
)(this,w,_d);ae.onMotionStopped=(function(be){return function(ee){be.width
=ee.target._pos/be.scale_factor;be.active_tweens.set_width=null;}
}
)(this);this.width=w;ae.start();}
}
return ae;}
;PureFW.Widget.get_height=function(){return Math.round(this.height*this.scale_factor);}
;PureFW.Widget.set_height=function(h,zd,$d,_d){h=parseInt(h);var ae=null;var xb=this.get_node();if(!xb){this.height=h;}
else{if(!zd){this.height=h;this.repaint();}
else{if(!this.active_tweens)this.active_tweens=new Object();if(this.active_tweens.set_height)this.active_tweens.set_height.stop();ae=new Tween(xb.style,'height',$d,this.get_height(),h*this.scale_factor,zd/1000,'px');this.active_tweens.set_height=ae;ae.onMotionFinished=(function(be,he,de){return function(){be.active_tweens.set_height=null;be.height=he;be.repaint();if(de)de.call(this);}
}
)(this,h,_d);ae.onMotionStopped=(function(be){return function(ee){be.height
=ee.target._pos/be.scale_factor;;be.active_tweens.set_height=null;}
}
)(this);ae.start();this.height=h;}
}
return ae;}
;PureFW.Widget.set_z_index=function(i){this.z_index=i;var xb=this.get_node();if(xb)xb.style.zIndex=i;}
;PureFW.Widget.get_z_index=function(){return this.z_index;}
;PureFW.Widget.set_positioning=function(vd){this.positioning=(vd=='absolute')?'absolute':'relative';var xb=this.get_node();if(xb)xb.style.position=this.positioning;}
;PureFW.Widget.get_positioning=function(vd){return this.positioning;}
;PureFW.Widget.set_float=function(tc){this.floating=(tc=='left'||tc=='right')?tc:'none';var xb=this.get_node();if(xb){xb.style.cssFloat=this.floating;xb.style.styleFloat=this.floating;}
}
;PureFW.Widget.set_css_class=function(ie){var xb=this.get_node();if(xb)xb.className=ie;}
PureFW.Widget.set_tooltip=function(vd){this.get_node().title=vd;}
PureFW.Widget.get_tooltip=function(){return this.get_node().title;}
PureFW.Widget.show=function(){var xb=this.get_node();if(xb)xb.style.display='block';this.on_show();}
;PureFW.Widget.hide=function(){var xb=this.get_node();if(xb)xb.style.display='none';this.on_hide();}
;PureFW.Widget.is_hidden=function(){var xb=this.get_node();if(xb)return(xb.style.display=='none');return true;}
;PureFW.Widget.fade_out=function(zd,je,$d){this.on_fade_out_begin();if(typeof(zd)=='undefined')zd=500;else
zd=parseInt(zd);$d=$d||null;je=je||0;if(!this.active_tweens)this.active_tweens=new Object();if(this.active_tweens.fade)this.active_tweens.fade.stop();var ae=new OpacityTween(this.get_node(),$d,this.get_opacity()*100,je*100,zd/1000
);this.active_tweens.fade=ae;ae.onMotionFinished=(function(be,ke){return function(){be.active_tweens.fade=null;be.set_opacity(ke);be.on_fade_out_end();}
}
)(this,je);ae.start();}
;PureFW.Widget.fade_in=function(zd,le,$d){this.on_fade_in_begin();if(typeof(zd)=='undefined')zd=500;else
zd=parseInt(zd);$d=$d||null;le=le||1;if(!this.active_tweens)this.active_tweens=new Object();if(this.active_tweens.fade)this.active_tweens.fade.stop();var ae=new OpacityTween(this.get_node(),$d,this.get_opacity()*100,le*100,zd/1000
);this.active_tweens.fade=ae;ae.onMotionFinished=(function(be,me){return function(){be.active_tweens.fade=null;be.set_opacity(me);be.on_fade_in_end();}
}
)(this,le);ae.start();}
;PureFW.Widget.deactivate=function(){this.activated=false;this.set_opacity(this._active_opacity-0.3);}
;PureFW.Widget.activate=function(){this.activated=true;this.set_opacity(this._active_opacity);}
;PureFW.Widget.is_activated=function(){return this.activated;}
PureFW.Widget.is_active=PureFW.Widget.is_activated;PureFW.Widget._register_event=function(tc){if(!document.all){this.get_node().setAttribute("on"+tc,"javascript: "+this.instance_name+".on_"+tc+"(event);");}
else{if(!this.ie_ev_workaround)this.ie_ev_workaround=new Object();this.ie_ev_workaround[tc]=new Object();this.ie_ev_workaround[tc].trys=10;this.ie_ev_workaround[tc].fn=(function(ne,oe){return function(){try{ne.get_node()['on'+oe]=function(rb){ne['on_'+oe](rb);}
}
catch(e){throw e;}
ne.ie_ev_workaround[oe].trys--;if(ne.ie_ev_workaround[oe].trys>0)PureFW.Timeout.set_timeout(ne.ie_ev_workaround[oe].fn,200);}
}
)(this,tc);PureFW.Timeout.set_timeout(this.ie_ev_workaround[tc].fn,200);}
}
PureFW.Widget.add_event_handler=function(tc,hc){if(typeof(tc)!=='string')throw new Error("Widget::add_event_handler: First argument 'type' "+"has to be of type 'string'");if(typeof(hc)!=='function')throw new Error("Widget::add_event_handler: Second argument 'fn' "+"has to be of type 'function'");if(tc=="destroy"){this.on_destroy_functions.push(hc);}
else if(tc=="click"){this.get_node().style.cursor="pointer";this.on_click_functions.push(hc);this._register_event("click");this._register_event("dblclick");this._register_event("mouseover");this._register_event("mouseout");}
else if(tc=="show"){this.on_show_functions.push(hc);}
else if(tc=="hide"){this.on_hide_functions.push(hc);}
else if(tc=="mouseover"){this.on_mouseover_functions.push(hc);this._register_event("mouseover");}
else if(tc=="mouseout"){this.on_mouseout_functions.push(hc);this._register_event("mouseout");}
else if(tc=="mousedown"){this.get_node().style.cursor="pointer";this.on_mousedown_functions.push(hc);this._register_event("mousedown");}
else if(tc=="mouseup"){this.get_node().style.cursor="pointer";this.on_mouseup_functions.push(hc);this._register_event("mouseup");}
else if(tc=="fade_out_begin"){this.on_fade_out_begin_functions.push(hc);}
else if(tc=="fade_out_end"){this.on_fade_out_end_functions.push(hc);}
else if(tc=="fade_in_begin"){this.on_fade_in_begin_functions.push(hc);}
else if(tc=="fade_in_end"){this.on_fade_in_end_functions.push(hc);}
else{try{PureFW.EventUtil.add_event_handler(this.get_node(),tc,hc);}
catch(e){throw new Error("Widget.add_event_handler("+tc+" ,"+hc+"): "+"getElem("+this.id+") has no properties.");}
}
}
;PureFW.Widget.remove_event_handler=function(tc,hc){if(typeof(tc)!=='string')throw new Error("Widget::remove_event_handler: First argument 'type' "+"has to be of type 'string'");if(typeof(hc)!=='function')throw new Error("Widget::remove_event_handler: Second argument 'fn' "+"has to be of type 'function'");if(tc=="destroy"){this.on_destroy_functions.remove(hc);}
else if(tc=="click"){this.on_click_functions.remove(hc);if(this.on_click_functions.length<=0)this.get_node().style.cursor='';}
else if(tc=="show"){this.on_show_functions.remove(hc);}
else if(tc=="hide"){this.on_hide_functions.remove(hc);}
else if(tc=="mouseover"){this.on_mouseover_functions.remove(hc);}
else if(tc=="mouseout"){this.on_mouseout_functions.remove(hc);}
else if(tc=="fade_out_begin"){this.on_fade_out_begin_functions.remove(hc);}
else if(tc=="fade_out_end"){this.on_fade_out_end_functions.remove(hc);}
else if(tc=="fade_in_begin"){this.on_fade_in_begin_functions.remove(hc);}
else if(tc=="fade_in_end"){this.on_fade_in_end_functions.remove(hc);}
else{PureFW.EventUtil.remove_event_handler(document.getElemById(this.id),tc,hc);}
}
;PureFW.Widget.clear_event_handler=function(tc){if(typeof(tc)!=='string')throw new Error("Widget::clear_event_handler: First argument 'type' "+"has to be of type 'string'");if(tc=="destroy"){this.on_destroy_functions.destroy();this.on_destroy_functions=new Array();}
else if(tc=="click"){this.on_click_functions.destroy();this.on_click_functions=new Array();this.get_node().style.cursor='';}
else if(tc=="show"){this.on_show_functions.destroy();this.on_show_functions=new Array();}
else if(tc=="hide"){this.on_hide_functions.destroy();this.on_hide_functions=new Array();}
else if(tc=="mouseover"){this.on_mouseover_functions.destroy();this.on_mouseover_functions=new Array();}
else if(tc=="mouseout"){this.on_mouseout_functions.destroy();this.on_mouseout_functions=new Array();}
}
PureFW.Widget.on_destroy=function(rb){if(!rb)rb=this.create_event("destroy");var n=this.on_destroy_functions.length;for(var i=0;i<n;i++){try{this.on_destroy_functions[i].call(this,rb);}
catch(e){}
}
}
;PureFW.Widget.on_click=function(rb){if(!this.activated)return;if(!rb)rb=this.create_event("click");var n=this.on_click_functions.length;for(var i=0;i<n;i++){this.on_click_functions[i].call(this,rb);}
}
;PureFW.Widget.on_dblclick=PureFW.Widget.on_click;PureFW.Widget.on_fade_in_begin=function(rb){if(!rb)rb=this.create_event("fade_in_begin");var n=this.on_fade_in_begin_functions.length;for(var i=0;i<n;i++){this.on_fade_in_begin_functions[i].call(this,rb);}
}
;PureFW.Widget.on_fade_in_end=function(rb){if(!rb)rb=this.create_event("fade_in_end");var n=this.on_fade_in_end_functions.length;for(var i=0;i<n;i++){this.on_fade_in_end_functions[i].call(this,rb);}
}
;PureFW.Widget.on_fade_out_begin=function(rb){if(!rb)rb=this.create_event("fade_out_begin");var n=this.on_fade_out_begin_functions.length;for(var i=0;i<n;i++){this.on_fade_out_begt_functions[i].call(this,rb);}
}
;PureFW.Widget.on_fade_out_end=function(rb){if(!rb)rb=this.create_event("fade_out_end");var n=this.on_fade_out_end_functions.length;for(var i=0;i<n;i++){this.on_fade_out_end_functions[i].call(this,rb);}
}
;PureFW.Widget.zoom=function(gd){if(gd<=0)throw new Error("PureFW.Widget.zoom: factor has to be > 0, "+gd
+" given");var pe=this.width*gd;var qe=this.height*gd;this.position.x+=(this.width-pe)>>1;this.position.y+=(this.height-qe)>>1;this.set_width(pe);this.set_height(qe);}
PureFW.Widget.on_mouseover=function(rb){if(!this.activated)return;if(!rb)rb=this.create_event("mouseover");var n=this.on_mouseover_functions.length;for(var i=0;i<n;i++){this.on_mouseover_functions[i].call(this,rb);}
}
;PureFW.Widget.on_mouseout=function(rb){if(!this.activated)return;if(!rb)rb=this.create_event("mouseout");var n=this.on_mouseout_functions.length;for(var i=0;i<n;i++){this.on_mouseout_functions[i].call(this,rb);}
}
;PureFW.Widget.on_mousedown=function(rb){if(!this.activated)return;if(!rb)rb=this.create_event("mousedown");var n=this.on_mousedown_functions.length;for(var i=0;i<n;i++){this.on_mousedown_functions[i].call(this,rb);}
}
;PureFW.Widget.on_mouseup=function(rb){if(!this.activated)return;if(!rb)rb=this.create_event("mouseup");var n=this.on_mouseup_functions.length;for(var i=0;i<n;i++){this.on_mouseup_functions[i].call(this,rb);}
}
;PureFW.Widget.on_show=function(rb){if(!rb)rb=this.create_event("show");var n=this.on_show_functions.length;for(var i=0;i<n;i++){this.on_show_functions[i].call(this,rb);}
}
;PureFW.Widget.on_hide=function(rb){if(!rb)rb=this.create_event("hide");var n=this.on_hide_functions.length;for(var i=0;i<n;i++){this.on_hide_functions[i].call(this,rb);}
}
;PureFW.Widget.get_id=function(){return this.id
}
;PureFW.Widget.set_id=function(wb){if(wb==this.id)return;if(document.getElemById(wb)){alert(PureFW.Widget.set_id.caller);throw new Error("Cannot set ID '"+wb+"'. Element with this ID "+"already defined in HTML-Document.");}
var xb=this.get_node();if(xb)xb.id=wb;this.id=wb;}
;PureFW.Widget.scale=function(gd){this.scale_factor=gd;if(this.quality!=='high'){this.pic_path=PureFW.PicPath.reference_path+'scales/'+this.scale_factor+'/quality/'+this.quality+'/';}
else{this.pic_path=PureFW.PicPath.reference_path+'scales/'+this.scale_factor+'/';}
this.repaint();}
;PureFW.Widget.set_quality=function(ed){this.quality=ed;if(this.quality!=='high'){this.pic_path=PureFW.PicPath.reference_path+'scales/'+this.scale_factor+'/quality/'+this.quality+'/';}
else
this.pic_path=PureFW.PicPath.reference_path+'scales/'+this.scale_factor+'/';this.repaint();}
;PureFW.Widget.set_pic_path=function(hd){this.pic_path=hd;this.repaint();}
;PureFW.Widget.get_pic_path=function(){return this.pic_path;}
;PureFW.Widget.add_pic_path=function(re){if((re.substr(0,7)=='http://')||(re.substr(0,8)=='https://'))return re;if(re.substr(0,3)=='../')return PureFW.PicPath.reference_path+re;else
return this.pic_path+re;}
PureFW.Widget.repaint=function(){var xb=this.get_node();if(xb){try{xb.style.width=this.get_width()+'px';}
catch(se){}
try{xb.style.height=this.get_height()+'px';}
catch(se){}
try{xb.style.left=this.get_x()+'px';}
catch(se){}
try{xb.style.top=this.get_y()+'px';}
catch(se){}
}
}
;PureFW.Widget.set_font_size=function(te){var xb=this.get_node();if(xb)xb.style.fontSize=(parseFloat(te)==te)?te+'em':te;}
;PureFW.Widget.set_text_shadow=function(ue,x,y,ve){this.get_node().style.textShadow=ue+" "+x+"px "+y+"px "+ve+"px ";}
PureFW.Widget.set_font_weight=function(we){var xb=this.get_node();if(xb)xb.style.fontWeight=we;}
;PureFW.Widget.set_font_style=function(xe){var xb=this.get_node();if(xb)xb.style.fontStyle=xe;}
PureFW.Widget.set_font_color=function(ye){var xb=this.get_node();if(xb)xb.style.color=ye;}
;PureFW.Widget.set_opacity=function(fd){var xb=this.get_node();if(fd<0)fd=0;if(fd>1)fd=1;this.opacity=fd;if(this.activated)this._active_opacity=fd;if(xb){if(document.all)return;xb.style.opacity=fd;xb.style.MozOpacity=fd;}
}
;PureFW.Widget.get_opacity=function(){return this.opacity;}
PureFW.Widget.insert_into_dom_tree=function(){}
;PureFW.Widget.destroy=function(){var xb=this.get_node();if(xb)xb.style.display='none';this.on_destroy();while(this.on_destroy_functions.length)this.on_destroy_functions.pop();while(this.on_click_functions.length)this.on_click_functions.pop();while(this.on_mouseover_functions.length)this.on_mouseover_functions.pop();while(this.on_mouseout_functions.length)this.on_mouseout_functions.pop();while(this.on_mousedown_functions.length)this.on_mousedown_functions.pop();while(this.on_mouseup_functions.length)this.on_mouseup_functions.pop();while(this.on_hide_functions.length)this.on_hide_functions.pop();while(this.on_show_functions.length)this.on_show_functions.pop();while(this.on_fade_out_begin_functions.length)this.on_fade_out_begin_functions.pop();while(this.on_fade_out_end_functions.length)this.on_fade_out_end_functions.pop();while(this.on_fade_in_begin_functions.length)this.on_fade_in_begin_functions.pop();while(this.on_fade_in_end_functions.length)this.on_fade_in_end_functions.pop();if(this.active_tweens){for(var i in this.active_tweens){if(this.active_tweens[i]){this.active_tweens[i].stopEnterFrame();delete this.active_tweens[i];this.active_tweens[i]=null;}
}
}
delete this.active_tweens;this.active_tweens=null;var ze=this.parent_node.childNodes;var $e=null;for(i=0;i<ze.length;i++){if(ze[i].id==this.id){$e=ze[i];break;}
}
if($e){this.parent_node.removeChild($e);}
PureFW.WidgetManager.manager_all.unregister_widget(this);try{delete this;}
catch(e){}
}
;PureFW.Widget.set_parent_node=function(_e){if(this.parent_node&&this.id&&(this.parent_node!==_e)){var ze=this.parent_node.childNodes;var $e=null;for(i=0;i<ze.length;i++){if(ze[i].id==this.id){$e=ze[i];break;}
}
if($e){this.parent_node.removeChild($e);_e.appendChild($e);}
}
this.parent_node=_e;}
;PureFW.Widget.get_parent_node=function(){return this.parent_node;}
;PureFW.Widget.instance_of=function(o){return PureFW.instance_of(this,o);}
;PureFW.Widget.get_node=function(){if(!this.id&&this.node)this.id=this.node.id;if(this.id){var af=document.getElemById(this.id);if(af)this.node=af;}
return this.node;}
;PureFW.Widget.create_event=function(tc,uc){return PureFW.EventUtil.create_event(tc,uc);}
;PureFW.Widget.disable_selection=function(sc){if(typeof(sc)=='undefined')sc=this.get_node();if(typeof sc.onselectstart!="undefined")sc.onselectstart=function(){return false}
;else if(typeof sc.style.MozUserSelect!="undefined")sc.style.MozUserSelect="none";else
sc.onmousedown=function(){return false}
;if(sc.style.cursor!='pointer')sc.style.cursor="default";}
PureFW.Image=function(xd,x,y,w,h,re,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,re,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.Image.instances["+this.instance_num+"]";this.id="Image"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.Image.extend(PureFW.Widget);PureFW.Image.num_instances=0;PureFW.Image.instances=new Array();PureFW.Image.prototype.init=function(xd,x,y,w,h,re,yd){PureFW.Image.parent.init.call(this,xd,x,y,w,h,yd);this.on_change_functions=new Array();this.pic_url=re||'';}
;PureFW.Image.prototype.insert_into_dom_tree=function(){PureFW.Image.parent.insert_into_dom_tree.call(this);this.node=document.createElement('img');this.node.style.position='absolute';this.node.style.width=this.get_width()+'px';this.node.style.height=this.get_height()+'px';this.node.style.left=this.get_x()+'px';this.node.style.top=this.get_y()+'px';this.node.style.backgroundRepeat='no-repeat';this.node.style.backgroundPosition='top left';this.node.style.overflow='hidden';this.node.src=this.add_pic_path(this.pic_url);this.node.id=this.id;this.parent_node.appendChild(this.node);}
;PureFW.Image.prototype.set_pic_url=function(re){if(this.pic_url==re)return;this.pic_url=re;if(this.get_node()){this.get_node().src=this.add_pic_path(this.pic_url);this.on_change();}
}
PureFW.Image.prototype.get_pic_url=function(){return this.pic_url;}
PureFW.Image.prototype.add_event_handler=function(tc,hc){if(tc==="change")this.on_change_functions.push(hc);else
PureFW.Image.parent.add_event_handler.call(this,tc,hc);}
;PureFW.Image.prototype.remove_event_handler=function(tc,hc){if(tc==="change")this.on_change_functions.remove(hc);else
PureFW.Image.parent.remove_event_handler.call(this,tc,hc);}
;PureFW.Image.prototype.on_change=function(rb){if(!rb){rb=this.create_event("change");}
var n=this.on_change_functions.length;for(var i=0;i<n;i++){this.on_change_functions[i].call(this,rb);}
}
;PureFW.Image.prototype.destroy=function(){while(this.on_change_functions.length)this.on_change_functions.pop();PureFW.Image.parent.destroy.call(this);}
;PureFW.Container=function(xd,x,y,w,h,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.Container.instances["+this.instance_num+"]";this.id="Container"+this.instance_num;this.bg_img_id="Container_bg_img"+this.instance_num;this.content_id="Container_cont"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.Container.extend(PureFW.Widget);PureFW.Container.num_instances=0;PureFW.Container.instances=new Array();PureFW.Container.prototype.init=function(xd,x,y,w,h,yd){PureFW.Container.parent.init.call(this,xd,x,y,w,h,yd);this.on_change_functions=new Array();this.bg_img=null;this.bg_img_node=null;this.node=null;this.content_node=null;this.added_widgets=new Array();}
;PureFW.Container.prototype.insert_into_dom_tree=function(){PureFW.Container.parent.insert_into_dom_tree.call(this);this.node=document.createElement('div');this.node.style.position='absolute';if(this.width>0)this.node.style.width=this.get_width()+'px';if(this.height>0)this.node.style.height=this.get_height()+'px';this.node.style.left=this.get_x()+'px';this.node.style.top=this.get_y()+'px';this.node.style.backgroundRepeat='no-repeat';this.node.style.backgroundPosition='top left';this.node.style.overflow='hidden';this.node.id=this.id;this.parent_node.appendChild(this.node);}
;PureFW.Container.prototype.set_overflow=function(s){this.get_node().style.overflow=s;this._get_content_node().style.overflow=s;}
PureFW.Container.prototype.set_width=function(w,zd,$d,_d){if((w>0)||zd)PureFW.Widget.set_width.call(this,w,zd,$d,_d);else{this.width=w;this.get_node().style.width='';}
}
;PureFW.Container.prototype.get_width=function(){if(this.width>0)return PureFW.Widget.get_width.call(this);else
return parseInt(this.get_node().offsetWidth);}
;PureFW.Container.prototype.set_height=function(h,zd,$d,_d){if((h>0)||zd)PureFW.Widget.set_height.call(this,h,zd,$d,_d);else{this.height=h;this.get_node().style.height='';}
}
;PureFW.Container.prototype.get_height=function(){if(this.height>0)return PureFW.Widget.get_height.call(this);else
return this.get_node().offsetHeight;}
;PureFW.Container.prototype.set_bg_color=function(ye){this._get_content_node().style.backgroundColor=ye;}
;PureFW.Container.prototype.set_text_align=function(bf){this._get_content_node().style.textAlign=bf;}
;PureFW.Container.prototype.set_bg_img=function(ub,cf){this.bg_img=ub;if(typeof(cf)==='undefined')cf=true;if(this.bg_img){var df=(this.bg_img.indexOf('://')>-1)?this.bg_img
:this.pic_path+this.bg_img;}
else
df=false
if(this.node){if(!cf){if(df)this.get_node().style.backgroundImage='url('+df+')';else
this.get_node().style.backgroundImage='';}
else{if(df){var ef=this.get_node().innerHTML;if(!this.content_node){this.node.innerHTML='';this.content_node=document.createElement('div');this.content_node.style.width="100%";this.content_node.style.height="100%";this.content_node.style.position="absolute";this.content_node.style.top='0';this.content_node.style.left='0';this.content_node.id=this.content_id;this.content_node.innerHTML=ef;this.content_node.style.zIndex=1;this.get_node().appendChild(this.content_node);}
else
this.content_node=document.getElemById(this.content_id);if(!this.bg_img_node){this.bg_img_node=document.createElement('img');this.bg_img_node.style.width="100%";this.bg_img_node.style.height="100%";this.bg_img_node.style.position='absolute';this.bg_img_node.style.top='0';this.bg_img_node.style.left='0';this.bg_img_node.style.zIndex=0;this.bg_img_node.id=this.bg_img_id;this.get_node().appendChild(this.bg_img_node);}
else
this.bg_img_node=document.getElemById(this.bg_img_id);if(this.bg_img_node)this.bg_img_node.src=df;}
else{if(this.bg_img_node=document.getElemById(this.bg_img_id))this.get_node().removeChild(this.bg_img_node);this.bg_img_node=null;}
}
}
}
;PureFW.Container.prototype.set_bg_repeat=function(ff){this.get_node().style.backgroundRepeat=ff;}
;PureFW.Container.prototype.set_bg_position=function(gf){this.get_node().style.backgroundPosition=gf;}
;PureFW.Container.prototype.get_bg_img=function(){return this.bg_img;}
;PureFW.Container.prototype.repaint=function(){var xb=this.get_node();if((typeof(this.scale_factor)=='undefined')&&!this.no_scale){this.scale_factor=PureFW.WidgetManager.manager_all.scale_factor;}
if(xb){if(this.width>0)xb.style.width=this.get_width()+'px';if(this.height>0)xb.style.height=this.get_height()+'px';xb.style.left=this.get_x()+'px';xb.style.top=this.get_y()+'px';}
this.set_bg_img(this.get_bg_img(),!!this.bg_img_node);}
;PureFW.Container.prototype.set_padding=function(hf){this._get_content_node().style.padding=hf+'px';}
;PureFW.Container.prototype.set_padding_left=function(p){this._get_content_node().style.paddingLeft=p+'px';}
;PureFW.Container.prototype.set_padding_right=function(p){this._get_content_node().style.paddingRight=p+'px';}
;PureFW.Container.prototype.set_padding_top=function(p){this._get_content_node().style.paddingTop=p+'px';}
;PureFW.Container.prototype.set_padding_bottom=function(p){this._get_content_node().style.paddingBottom=p+'px';}
;PureFW.Container.prototype.set_content=function(text,yb){this.remove_all_widgets();this.on_change(this.create_event("change"));document.setElemInnerHTML(this._get_content_node(),text,yb);}
;PureFW.Container.prototype.add_widget=function(jf,x,y,w,h,kf,lf,mf,nf,of,pf,qf,rf){var w=new jf(this._get_content_node(),x,y,w,h,kf,lf,mf,nf,of,pf,qf,rf
);this.added_widgets.push(w);this.on_change(this.create_event("change"));return w;}
;PureFW.Container.prototype.remove_all_widgets=function(){this.on_change(this.create_event("change"));this.added_widgets.destroy();}
PureFW.Container.prototype.add_content=function(text){this.on_change(this.create_event("change"));document.setElemInnerHTML(this._get_content_node(),this._get_content_node().innerHTML+text);}
;PureFW.Container.prototype.get_content=function(){if(document.getElemById(this.content_id))this.content_node=document.getElemById(this.content_id);if(document.getElemById(this.id))this.node=document.getElemById(this.id);return(this.content_node)?this.content_node.innerHTML
:this.node.innerHTML;}
;PureFW.Container.prototype.get_content_node=function(){if(document.getElemById(this.content_id))this.content_node=document.getElemById(this.content_id);if(document.getElemById(this.id))this.node=document.getElemById(this.id);return(this.content_node)?this.content_node:this.node;}
;PureFW.Container.prototype._get_content_node=function(){return this.get_content_node();}
;PureFW.Container.prototype.add_event_handler=function(tc,hc){if(tc==="change")this.on_change_functions.push(hc);else
PureFW.Container.parent.add_event_handler.call(this,tc,hc);}
;PureFW.Container.prototype.remove_event_handler=function(tc,hc){if(tc==="change")this.on_change_functions.remove(hc);else
PureFW.Container.parent.remove_event_handler.call(this,tc,hc);}
;PureFW.Container.prototype.on_change=function(rb){if(!rb){rb=this.create_event("change");}
var n=this.on_change_functions.length;for(var i=0;i<n;i++){this.on_change_functions[i].call(this,rb);}
}
;PureFW.Container.prototype.destroy=function(){this.on_change_functions.destroy();this.added_widgets.destroy();PureFW.Container.parent.destroy.call(this);}
;PureFW.CollapseContainer=function(xd,x,y,w,h){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.CollapseContainer.instances["+this.instance_num+"]";this.id="CollapseContainer"+this.instance_num;this.bg_img_id="CollapseContainer_bg_img"+this.instance_num;this.content_id="CollapseContainer_cont"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.CollapseContainer.extend(PureFW.Container);PureFW.CollapseContainer.num_instances=0;PureFW.CollapseContainer.instances=new Array();PureFW.CollapseContainer.prototype.init=function(xd,x,y,w,h){PureFW.CollapseContainer.parent.init.call(this,xd,x,y,w,h);this.on_collapse_functions=new Array();this.on_expand_functions=new Array();this.arrow_button=null;this.root_container=null;this.body_container=null;this.big_arrow_url='ui/backgrounds/faq/arrows/arrow_big_';this.small_arrow_url='ui/backgrounds/faq/arrows/arrow_small_';this.current_arrow_url=this.big_arrow_url;}
;PureFW.CollapseContainer.prototype.insert_into_dom_tree=function(){PureFW.CollapseContainer.parent.insert_into_dom_tree.call(this);this.get_node().style.display='block';this.arrow_button=new PureFW.Image(this,0,0,17,11);this.arrow_button.set_pic_url(this.current_arrow_url+'down.png');this.arrow_button.set_positioning('relative');this.arrow_button.set_float("left");this.arrow_button.parent_container=this;this.arrow_button.add_event_handler("click",(function(be){return function(rb){be.toggle_expand_collapse();}
}
)(this));this.root_container=new PureFW.Container(this,0,0,this.width-this.arrow_button.width,20);this.root_container.set_positioning('relative');this.root_container.set_float('left');this.root_container.parent_container=this;this.root_container.add_event_handler("click",(function(be){return function(rb){be.toggle_expand_collapse();}
}
)(this));this.body_container=new PureFW.Container(this,0,0,this.width-this.arrow_button.width,0);this.body_container.set_positioning('relative');this.body_container.set_float('left');this.body_container.set_x(20);this.body_container.parent_container=this;this.collapse_body();this.get_node().style.clear='both';}
;PureFW.CollapseContainer.prototype.get_body=function(){return this.body_container.get_content_node();}
PureFW.CollapseContainer.prototype.toggle_expand_collapse=function(){if(this.body_container.is_hidden())this.expand_body();else
this.collapse_body();}
PureFW.CollapseContainer.prototype.expand_body=function(){this.arrow_button.set_pic_url(this.current_arrow_url+'up.png');this.body_container.show();this.on_expand();}
PureFW.CollapseContainer.prototype.collapse_body=function(){this.arrow_button.set_pic_url(this.current_arrow_url+'down.png');this.body_container.hide();this.on_collapse();}
PureFW.CollapseContainer.prototype.set_arrow=function(sf){if(sf=='small'){this.current_arrow_url=this.small_arrow_url;this.arrow_button.set_width(13);this.arrow_button.set_height(9);this.arrow_button.set_y(3);}
else if(sf=='big'){this.current_arrow_url=this.big_arrow_url;this.arrow_button.set_width(17);this.arrow_button.set_height(11);this.arrow_button.set_y(0);}
}
PureFW.CollapseContainer.prototype.set_content=function(text,yb){this.on_change(this.create_event("change"));document.setElemInnerHTML(this.root_container.get_content_node(),text,yb);}
;PureFW.CollapseContainer.prototype.add_content=function(text){this.on_change(this.create_event("change"));document.setElemInnerHTML(this.root_container.get_content_node(),this.root_container.get_content_node().innerHTML+text);}
;PureFW.CollapseContainer.prototype.get_content=function(){if(document.getElemById(this.content_id))this.root_container.content_node=document.getElemById(this.content_id);if(document.getElemById(this.id))this.node=document.getElemById(this.id);if(this.root_container.content_node){return this.root_container.content_node.innerHTML;}
else
{return this.root_container.node.innerHTML;}
}
;PureFW.CollapseContainer.prototype.add_event_handler=function(tc,hc){if(tc==="collapse")this.on_collapse_functions.push(hc);else if(tc==="expand")this.on_expand_functions.push(hc);else
PureFW.Container.parent.add_event_handler.call(this,tc,hc);}
;PureFW.CollapseContainer.prototype.remove_event_handler=function(tc,hc){if(tc==="collapse")this.on_collapse_functions.remove(hc);else if(tc==="expand")this.on_expand_functions.remove(hc);else
PureFW.Container.parent.remove_event_handler.call(this,tc,hc);}
;PureFW.CollapseContainer.prototype.on_collapse=function(rb){if(!rb){rb=this.create_event("collapse");}
var n=this.on_collapse_functions.length;for(var i=0;i<n;i++){this.on_collapse_functions[i].call(this,rb);}
}
;PureFW.CollapseContainer.prototype.on_expand=function(rb){if(!rb){rb=this.create_event("expand");}
var n=this.on_expand_functions.length;for(var i=0;i<n;i++){this.on_expand_functions[i].call(this,rb);}
}
;PureFW.CollapseContainer.prototype.destroy=function(){while(this.on_expand_functions.length)this.on_expand_functions.pop();while(this.on_collapse_functions.length)this.on_collapse_functions.pop();PureFW.Container.parent.destroy.call(this);}
;PureFW.Checkbox=function(xd,x,y,w,h,re,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,re,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.Checkbox.instances["+this.instance_num+"]";this.id="Checkbox"+this.instance_num;this.pic_id="Checkbox_pic"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.Checkbox.extend(PureFW.Widget);PureFW.Checkbox.num_instances=0;PureFW.Checkbox.instances=new Array();PureFW.Checkbox.prototype.init=function(xd,x,y,w,h,re,yd){PureFW.Checkbox.parent.init.call(this,xd,x,y,w,h,yd);this.pic_url=re;this.on_check_functions=new Array();this.on_uncheck_functions=new Array();this.node=null;this.pic_node=null;this.checked=false;}
;PureFW.Checkbox.prototype.insert_into_dom_tree=function(){PureFW.Checkbox.parent.insert_into_dom_tree.call(this);this.node=document.createElement('div');this.node.style.position='absolute';this.node.style.width=this.get_width()+'px';this.node.style.height=this.get_height()+'px';this.node.style.left=this.get_x()+'px';this.node.style.top=this.get_y()+'px';this.node.style.backgroundRepeat='no-repeat';this.node.style.backgroundPosition='top left';this.node.style.overflow='hidden';this.node.id=this.id;this.pic_node=document.createElement('img');this.pic_node.style.position='absolute';this.pic_node.style.width='100%';this.pic_node.style.height='200%';this.pic_node.style.left=0;this.pic_node.style.top=0;this.pic_node.src=this.add_pic_path(this.pic_url);this.pic_node.id=this.pic_id;this.parent_node.appendChild(this.node);this.node.appendChild(this.pic_node);this.add_event_handler("click",(function(tf){return function(rb){tf.toggle()}
}
)(this));}
;PureFW.Checkbox.prototype.get_pic_node=function(){if(!this.pic_id&&this.pic_node)this.pic_id=this.pic_node.id;if(this.pic_id){var af=document.getElemById(this.pic_id);if(af)this.pic_node=af;}
return this.pic_node;}
;PureFW.Checkbox.prototype.set_pic_url=function(re){this.pic_url=re;this.get_pic_node().src=this.add_pic_path(this.pic_url);}
;PureFW.Checkbox.prototype.check=function(){if(!this.activated)return;this.get_pic_node().style.top=-this.get_height()+"px";this.checked=true;this.on_check();}
;PureFW.Checkbox.prototype.uncheck=function(){if(!this.activated)return;this.get_pic_node().style.top=0;this.checked=false;this.on_uncheck();}
;PureFW.Checkbox.prototype.toggle=function(){if(this.checked)this.uncheck();else
this.check();}
;PureFW.Checkbox.prototype.is_checked=function(){return this.checked;}
;PureFW.Checkbox.prototype.add_event_handler=function(tc,hc){if(tc==="check")this.on_check_functions.push(hc);else if(tc==="uncheck")this.on_uncheck_functions.push(hc);else
PureFW.Image.parent.add_event_handler.call(this,tc,hc);}
;PureFW.Checkbox.prototype.remove_event_handler=function(tc,hc){if(tc==="check")this.on_check_functions.remove(hc);else if(tc==="uncheck")this.on_uncheck_functions.remove(hc);else
PureFW.Image.parent.remove_event_handler.call(this,tc,hc);}
;PureFW.Checkbox.prototype.on_check=function(rb){if(!rb){rb=this.create_event("check");}
var n=this.on_check_functions.length;for(var i=0;i<n;i++){this.on_check_functions[i].call(this,rb);}
}
;PureFW.Checkbox.prototype.on_uncheck=function(rb){if(!rb){rb=this.create_event("uncheck");}
var n=this.on_uncheck_functions.length;for(var i=0;i<n;i++){this.on_uncheck_functions[i].call(this,rb);}
}
;PureFW.Checkbox.prototype.destroy=function(){while(this.on_check_functions.length)this.on_check_functions.pop();while(this.on_uncheck_functions.length)this.on_uncheck_functions.pop();PureFW.Checkbox.parent.destroy.call(this);}
;PureFW.RadioButton=function(xd,x,y,w,h,re){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,re);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.RadioButton.instances["+this.instance_num+"]";this.id="RadioButton"+this.instance_num;this.pic_id="RadioButton_pic"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.RadioButton.extend(PureFW.Checkbox);PureFW.RadioButton.num_instances=0;PureFW.RadioButton.instances=new Array();PureFW.RadioButton.prototype.init=function(xd,x,y,w,h,re){PureFW.RadioButton.parent.init.call(this,xd,x,y,w,h,re);this.radio_button_links=new Array();this.group_visit_counter=0;}
;PureFW.RadioButton.prototype.insert_into_dom_tree=function(){PureFW.RadioButton.parent.insert_into_dom_tree.call(this);while(this.on_click_functions.length)this.on_click_functions.pop();this.add_event_handler("click",(function(tf){return function(rb){tf.group_visit_counter=1;tf.uncheck_all_in_group();tf.check();}
}
)(this));}
;PureFW.RadioButton.prototype.link_with_radio_button=function(uf){this.radio_button_links.push(uf);}
;PureFW.RadioButton.prototype.uncheck_all_in_group=function(){if(this.group_visit_counter==1){this.group_visit_counter=2;}
else if(this.group_visit_counter>1){this.group_visit_counter=0;return;}
var n=this.radio_button_links.length;for(var i=0;i<n;i++){this.radio_button_links[i].uncheck();this.radio_button_links[i].uncheck_all_in_group();}
}
;PureFW.DropMenu=function(xd,x,y,w,h,vf,wf,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,vf,wf,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.DropMenu.instances["+this.instance_num+"]";this.id="DropMenu"+this.instance_num;this.content_id="DropMenu_cont"+this.instance_num;this.bg_img_id="DropMenu_bg"+this.instance_num;this.under_section_id="DropMenuItems"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.DropMenu.extend(PureFW.Container);PureFW.DropMenu.num_instances=0;PureFW.DropMenu.instances=new Array();PureFW.DropMenu.prototype.init=function(xd,x,y,w,h,vf,wf,yd){PureFW.DropMenu.parent.init.call(this,xd,x,y,w,h,yd);this.current_item_container=null;this.under_section_container=null;this.under_section_items=new Array();this.under_background_top_left=null;this.under_background_top_middle=null;this.under_background_top_right=null;this.under_background_bottom_left=null;this.under_background_bottom_middle=null;this.under_background_bottom_right=null;this.left_part_container=null;this.middle_part_container=null;this.right_part_container=null;this.item_height=h-10;this.max_visible_items=5;this.left_width=vf||6;this.right_width=wf||16;this.menu_open=false;}
;PureFW.DropMenu.prototype.insert_into_dom_tree=function(){PureFW.DropMenu.parent.insert_into_dom_tree.call(this);this.node.style.overflow='visible';this.left_part_container=new PureFW.Container(this.get_node(),0,0,this.left_width,this.height
);this.left_part_container.set_bg_img('ui/elements/form/drop_down/left_'+this.left_width+'x'+this.height+'.png');this.left_part_container.get_node().style.cursor='pointer';this.left_part_container.add_event_handler("click",(function(be){return function(rb){be.toggle_drop_menu();}
}
)(this));this.left_part_container.disable_selection();this.left_part_container.parent_container=this;this.middle_part_container=new PureFW.Container(this.get_node(),this.left_part_container.width,0,this.width-(this.left_width+this.right_width),this.height
);this.middle_part_container.set_bg_img('ui/elements/form/drop_down/middle_DYNx'+this.height+'.png');this.middle_part_container.get_node().style.cursor='pointer';this.middle_part_container.add_event_handler("click",(function(be){return function(rb){be.toggle_drop_menu();}
}
)(this));this.middle_part_container.disable_selection();this.middle_part_container.parent_container=this;this.right_part_container=new PureFW.Container(this.get_node(),this.middle_part_container.width+this.left_part_container.width,0,this.right_width,this.height);this.right_part_container.set_bg_img('ui/elements/form/drop_down/right_'+this.right_width+'x'+this.height+'.png');this.right_part_container.get_node().style.cursor='pointer';this.right_part_container.add_event_handler("click",(function(be){return function(rb){be.toggle_drop_menu();}
}
)(this));this.right_part_container.disable_selection();this.right_part_container.parent_container=this;this.current_item_container=new PureFW.Container(this.middle_part_container.get_node(),0,6,this.middle_part_container.width,this.item_height);this.current_item_container.set_text_align('center');this.current_item_container.set_font_color('#366A9B');this.current_item_container.set_font_weight('bold');this.current_item_container.parent_container=this.middle_part_container;this.under_section_container=new PureFW.ScrollContainer(this.get_node(),this.left_part_container.width,this.height,this.width-(this.left_part_container.width+this.right_part_container.width),0);this.under_section_container.set_id(this.under_section_id);this.under_section_container.set_font_color('#366A9B');this.under_section_container.hide();this.under_section_container.set_z_index(40);this.under_section_container.parent_container=this;this.under_section_container.get_content_node().style.zIndex=2;this.under_background_top_left=new PureFW.Image(this.under_section_container.get_node(),0,0,5,0,'ui/elements/form/drop_down/top_left.png');this.under_background_top_middle=new PureFW.Image(this.under_section_container.get_node(),5,0,0,0,'ui/elements/form/drop_down/top_middle.png');this.under_background_top_right=new PureFW.Image(this.under_section_container.get_node(),0,0,5,0,'ui/elements/form/drop_down/top_right.png');this.under_background_bottom_left=new PureFW.Image(this.under_section_container.get_node(),0,0,5,6,'ui/elements/form/drop_down/bottom_left.png');this.under_background_bottom_middle=new PureFW.Image(this.under_section_container.get_node(),5,0,0,6,'ui/elements/form/drop_down/bottom_middle.png');this.under_background_bottom_right=new PureFW.Image(this.under_section_container.get_node(),0,0,5,6,'ui/elements/form/drop_down/bottom_right.png');this.resize_backgrounds();}
;PureFW.DropMenu.prototype.add_item=function(name,hc){var xf=this.under_section_items.length;var yf=this.add_widget_as_item(PureFW.Container,[],hc
);yf.set_text_align('center');yf.set_content(name);if(xf==0){this.current_item_container.set_content(this.under_section_items[xf].get_content());}
}
;PureFW.DropMenu.prototype.add_widget_as_item=function(zf,$f,_f){var xf=this.under_section_items.length;this.under_section_items[xf]=this.under_section_container.add_widget(zf,0,this.item_height*xf,this.under_section_container.width,this.item_height,$f[0],$f[1],$f[2],$f[3],$f[4],$f[5],$f[6],$f[7]);this.under_section_items[xf].add_event_handler("click",(function(z,tf){return function(rb){tf.select_item(z);}
}
)(xf,this));if(this.under_section_items.length>this.max_visible_items){this.under_section_container.set_height(this.item_height*this.max_visible_items);}
else
{this.under_section_container.set_height(this.item_height*this.under_section_items.length);}
this.under_section_container.set_inner_height(this.item_height*this.under_section_items.length);this.resize_backgrounds();if(_f){this.under_section_items[xf].add_event_handler("click",_f);}
return this.under_section_items[xf];}
PureFW.DropMenu.prototype.select_by_name=function(ag){this.select_item(this.get_index_of(ag));}
;PureFW.DropMenu.prototype.get_index_of=function(ag){var n=this.under_section_items.length;for(var i=0;i<n;i++){var bg=this.under_section_items[i].get_content();if(ag==bg){return i;}
}
}
;PureFW.DropMenu.prototype.scroll_to_item=function(cg){var dg=this.get_index_of(cg);this.under_section_container.scroll_to(0,dg*this.item_height);}
;PureFW.DropMenu.prototype.resize_backgrounds=function(){if((this.under_section_container.height<this.under_background_bottom_left.height)){return;}
this.under_background_top_left.set_height((this.under_section_container.height-this.under_background_bottom_left.height));this.under_background_top_middle.set_height((this.under_section_container.height-this.under_background_bottom_left.height));this.under_background_top_middle.set_width(this.under_section_container.width-(this.under_background_top_left.width+this.under_background_top_right.width));this.under_background_top_right.set_height((this.under_section_container.height-this.under_background_bottom_left.height));this.under_background_top_right.set_x(this.under_background_top_middle.width+this.under_background_top_left.width);this.under_background_bottom_left.set_y(this.under_background_top_left.height);this.under_background_bottom_middle.set_y(this.under_background_top_left.height);this.under_background_bottom_middle.set_width(this.under_section_container.width-(this.under_background_top_left.width+this.under_background_top_right.width));this.under_background_bottom_right.set_y(this.under_background_top_left.height);this.under_background_bottom_right.set_x(this.under_background_top_middle.width+this.under_background_top_left.width);}
;PureFW.DropMenu.prototype.set_maximum_visible_items=function(eg){this.max_visible_items=eg;if(this.max_visible_items>this.under_section_items.length){this.under_section_container.set_height(this.item_height*this.max_visible_items);}
else
{this.under_section_container.set_height(this.item_height*this.under_section_items.length);}
}
;PureFW.DropMenu.prototype.toggle_drop_menu=function(){if(this.menu_open)this.close_drop_menu();else
this.open_drop_menu();}
PureFW.DropMenu.prototype.open_drop_menu=function(){this.under_section_container.show();this.scroll_to_item(this.current_item_container.get_content());this.menu_open=true;}
;PureFW.DropMenu.prototype.close_drop_menu=function(){this.under_section_container.hide();this.under_section_container.set_inner_top(0);this.menu_open=false;}
;PureFW.DropMenu.prototype.select_item=function(fg){try
{this.current_item_container.set_content(this.under_section_items[fg].get_content());this.close_drop_menu();}
catch(e){}
}
;PureFW.DropMenu.prototype.set_width=function(w){PureFW.Container.set_width.call(this,w);this.under_section_container.set_width(w-25);this.middle_part_container.set_width(w-39);this.right_part_container.set_x(this.middle_part_container.width+26);var n=this.under_section_items.length;for(var i=0;i<n;i++){this.under_section_items[i].set_width(w-25);}
}
;PureFW.DropMenu.prototype.set_height=function(h){PureFW.Container.set_height.call(this,h);gg=h-10;this.under_section_container.set_height(this.item_height);var n=this.under_section_items.length;for(var i=0;i<n;i++){this.under_section_items[i].set_height(this.item_height);}
}
;PureFW.DropMenu.prototype.set_item_padding=function(hg){this.current_item_container.set_padding_left(hg);this.current_item_container.set_padding_right(hg);var n=this.under_section_items.length;for(var i=0;i<n;i++){this.under_section_items[i].set_padding_left(hg);this.under_section_items[i].set_padding_right(hg);}
}
;PureFW.DropMenu.prototype.set_text_align=function(bf){this.current_item_container.set_text_align(bf);var n=this.under_section_items.length;for(var i=0;i<n;i++){this.under_section_items[i].set_text_align(bf);}
}
PureFW.ScrollContainer=function(xd,x,y,w,h){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h);this.instance_num=PureFW.ScrollContainer.num_instances;this.instance_name="PureFW.ScrollContainer.instances["+this.instance_num+"]";this.id="ScrollContainer"+this.instance_num;this.bg_img_id="ScrollContainer_bg_img"+this.instance_num;this.inner_container_id="ScrollContent"+this.instance_num;this.vertical_scroller_id="VertiScroller"+this.instance_num;this.horizontal_scroller_id="HorizonScroller"+this.instance_num;this.insert_into_dom_tree();PureFW.ScrollContainer.num_instances++;PureFW.ScrollContainer.instances[this.instance_num]=this;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.ScrollContainer.extend(PureFW.Container);PureFW.ScrollContainer.num_instances=0;PureFW.ScrollContainer.instances=new Array();PureFW.ScrollContainer.active_scrolling_instance=null;PureFW.ScrollContainer.active_dragging_instance=null;PureFW.ScrollContainer.while_vertical_dragging=function(rb){PureFW.ScrollContainer.active_dragging_instance._calculate_vertical_value(rb.detail);}
;PureFW.ScrollContainer.while_horizontal_dragging=function(rb){PureFW.ScrollContainer.active_dragging_instance._calculate_horizontal_value(rb.detail);}
;PureFW.ScrollContainer.vertical_scroller_dragged=function(rb){PureFW.ScrollContainer.active_dragging_instance._start_vertical_dragging();}
;PureFW.ScrollContainer.vertical_scroller_dropped=function(rb){PureFW.ScrollContainer.active_dragging_instance._stop_vertical_dragging();}
;PureFW.ScrollContainer.horizontal_scroller_dragged=function(rb){PureFW.ScrollContainer.active_dragging_instance._start_horizontal_dragging();}
;PureFW.ScrollContainer.horizontal_scroller_dropped=function(rb){PureFW.ScrollContainer.active_dragging_instance._stop_horizontal_dragging();}
;PureFW.ScrollContainer.while_scrolling=function(bd){PureFW.ScrollContainer.active_scrolling_instance.scroll(bd);}
;PureFW.ScrollContainer.up_arrow_clicked=function(){PureFW.ScrollContainer.active_scrolling_instance.scroll_up();}
;PureFW.ScrollContainer.down_arrow_clicked=function(){PureFW.ScrollContainer.active_scrolling_instance.scroll_down();}
;PureFW.ScrollContainer.left_arrow_clicked=function(){PureFW.ScrollContainer.active_scrolling_instance.scroll_left();}
;PureFW.ScrollContainer.right_arrow_clicked=function(){PureFW.ScrollContainer.active_scrolling_instance.scroll_right();}
;PureFW.ScrollContainer.prototype.init=function(xd,x,y,w,h){PureFW.ScrollContainer.parent.init.call(this,xd,x,y,w,h);this.inner_container=null;this.vertical_scrolling_enabled=false;this.horizontal_scrolling_enabled=false;this.hiding_allowed=null;this.button_size=0;this.scroller_color=null;this.up_arrow_container=null;this.down_arrow_container=null;this.side_scroller_container=null;this.side_scroller=null;this.left_arrow_container=null;this.right_arrow_container=null;this.bottom_scroller_container=null;this.bottom_scroller=null;this.internal_scroll_top=null;this.internal_scroll_middle_vertical=null;this.internal_scroll_bottom=null;this.internal_scroll_left=null;this.internal_scroll_middle_horizontal=null;this.internal_scroll_right=null;this.vertical_scroller_size=0;this.vertical_scroller_position=0;this.vertical_scroller_proportion=0;this.vertical_scroll_interval=.2;this.horizontal_scroller_size=0;this.horizontal_scroller_position=0;this.horizontal_scroller_proportion=0;this.horizontal_scroll_interval=.2;this.max_vertical_scroll=0;this.max_horizontal_scroll=0;this.on_scroll=null;this.slide_scroll_interval=null;this.inner_container_id=null;this.vertical_scroller_id=null;this.horizontal_scroller_id=null;}
;PureFW.ScrollContainer.prototype.insert_into_dom_tree=function(){this.button_size=16;this.scroller_color='default';PureFW.ScrollContainer.parent.insert_into_dom_tree.call(this);this.vertical_scrolling_enabled=true;this.horizontal_scrolling_enabled=true;this.hiding_allowed=true;this.node.style.overflow='hidden';this.set_id(this.id);this.inner_container=new PureFW.Container(this.node,0,0,this.width,this.height);this.inner_container.set_id(this.inner_container_id);this.inner_container.parent_container=this;this.up_arrow_container=new PureFW.Image(this.node,0,0,this.button_size,this.button_size,'ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowtop.png');this.up_arrow_container.set_positioning('absolute');this.up_arrow_container.set_x(this.width-this.button_size);this.up_arrow_container.set_z_index(10);this.up_arrow_container.add_event_handler("click",PureFW.ScrollContainer.up_arrow_clicked);this.up_arrow_container.add_event_handler("mousedown",(function(be){return function(rb){be.slide_scroll_interval=PureFW.Timeout.set_interval(function(){be.scroll_up()}
,100);}
}
)(this));this.up_arrow_container.add_event_handler("mouseup",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.up_arrow_container.add_event_handler("mouseout",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.up_arrow_container.parent_container=this;this.down_arrow_container=new PureFW.Image(this.node,0,0,this.button_size,this.button_size,'ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowbottom.png');this.down_arrow_container.set_positioning('absolute');this.down_arrow_container.set_x(this.width-this.button_size);this.down_arrow_container.set_y(this.height-this.button_size);this.down_arrow_container.set_z_index(10);this.down_arrow_container.add_event_handler("click",PureFW.ScrollContainer.down_arrow_clicked);this.down_arrow_container.add_event_handler("mousedown",(function(be){return function(rb){be.slide_scroll_interval=PureFW.Timeout.set_interval(function(){be.scroll_down();}
,100
);}
}
)(this));this.down_arrow_container.add_event_handler("mouseup",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.down_arrow_container.add_event_handler("mouseout",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.down_arrow_container.parent_container=this;this.side_scroller_container=new PureFW.Container(this.node,0,0,this.button_size,this.height-2*this.button_size);this.side_scroller_container.set_positioning('absolute');this.side_scroller_container.set_x(this.width-this.button_size);this.side_scroller_container.set_y(this.button_size);this.side_scroller_container.parent_container=this;if(this.inner_container.height>0){this.vertical_scroller_size=(this.side_scroller_container.height)*(this.side_scroller_container.height/this.inner_container.height);}
this.vertical_scroller_proportion=1;this.side_scroller=new PureFW.Container(this.side_scroller_container.get_content_node(),0,0,this.button_size,this.vertical_scroller_size);this.side_scroller.set_z_index(9);this.side_scroller.set_id(this.vertical_scroller_id);this.side_scroller.add_event_handler("mousedown",(function(be){return function(rb){be._start_vertical_dragging();}
}
)(this));this.side_scroller.add_event_handler("mouseup",(function(be){return function(rb){be._stop_vertical_dragging();}
}
)(this));this.side_scroller_container.parent_container=this.side_scroller_container;this.internal_scroll_top=new PureFW.Container(this.side_scroller,0,0,this.button_size,10);this.internal_scroll_top.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_top.png');this.internal_scroll_top.parent_container=this.side_scroller;this.internal_scroll_middle_vertical=new PureFW.Container(this.side_scroller,0,0,this.button_size,1);this.internal_scroll_middle_vertical.set_y(10);this.internal_scroll_middle_vertical.set_height(this.vertical_scroller_size-20);this.internal_scroll_middle_vertical.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_middle_vertical.png');this.internal_scroll_middle_vertical.parent_container=this.side_scroller;this.internal_scroll_bottom=new PureFW.Container(this.side_scroller,0,0,this.button_size,10);this.internal_scroll_bottom.set_y(this.internal_scroll_middle_vertical.height+10);this.internal_scroll_bottom.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_bottom.png');this.internal_scroll_bottom.parent_container=this.side_scroller;this.left_arrow_container=new PureFW.Container(this.node,0,0,this.button_size,this.button_size);this.left_arrow_container.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowleft.png');this.left_arrow_container.set_positioning('absolute');this.left_arrow_container.set_y(this.height-this.button_size);this.left_arrow_container.set_z_index(10);this.left_arrow_container.add_event_handler("click",PureFW.ScrollContainer.left_arrow_clicked);this.left_arrow_container.add_event_handler("mousedown",(function(be){return function(rb){be.slide_scroll_interval=PureFW.Timeout.set_timeout(function(){be.scroll_left()}
,100);}
}
)(this));this.left_arrow_container.add_event_handler("mouseup",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.left_arrow_container.add_event_handler("mouseout",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.left_arrow_container.parent_container=this;this.right_arrow_container=new PureFW.Container(this.node,0,0,this.button_size,this.button_size);this.right_arrow_container.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowright.png');this.right_arrow_container.set_positioning('absolute');this.right_arrow_container.set_x(this.width-2*this.button_size);this.right_arrow_container.set_y(this.height-this.button_size);this.right_arrow_container.set_z_index(10);this.right_arrow_container.add_event_handler("click",PureFW.ScrollContainer.right_arrow_clicked);this.right_arrow_container.add_event_handler("mousedown",(function(be){return function(rb){be.slide_scroll_interval=PureFW.Timeout.set_interval(function(){be.scroll_right();}
,100);}
}
)(this));this.right_arrow_container.add_event_handler("mouseup",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.right_arrow_container.add_event_handler("mouseout",(function(be){return function(rb){PureFW.Timeout.clear_interval(be.slide_scroll_interval);}
}
)(this));this.right_arrow_container.parent_container=this;this.bottom_scroller_container=new PureFW.Container(this.node,0,0,this.width-3*this.button_size,this.button_size);this.bottom_scroller_container.set_positioning('absolute');this.bottom_scroller_container.set_x(this.button_size);this.bottom_scroller_container.set_y(this.height-this.button_size);this.bottom_scroller_container.parent_container=this;if(this.inner_container.width>0){this.horizontal_scroller_size=(this.bottom_scroller_container.width)*(this.bottom_scroller_container.width/this.inner_container.width);}
this.horizontal_scroller_proportion=1;this.bottom_scroller=new PureFW.Container(this.bottom_scroller_container,0,0,this.horizontal_scroller_size,this.button_size);this.bottom_scroller.set_z_index(9);this.bottom_scroller.set_id(this.horizontal_scroller_id);this.bottom_scroller.add_event_handler("mousedown",(function(be){return function(rb){be._start_horizontal_dragging();}
}
)(this));this.bottom_scroller.add_event_handler("mouseup",(function(be){return function(rb){be._stop_horizontal_dragging();}
}
)(this));this.bottom_scroller.parent_container=this.bottom_scroller_container;this.internal_scroll_left=new PureFW.Container(this.bottom_scroller,0,0,10,this.button_size);this.internal_scroll_left.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_left.png');this.internal_scroll_left.parent_container=this.bottom_scroller;this.internal_scroll_middle_horizontal=new PureFW.Container(this.bottom_scroller,0,0,1,this.button_size);this.internal_scroll_middle_horizontal.set_x(10);this.internal_scroll_middle_horizontal.set_width(this.horizontal_scroller_size-20);this.internal_scroll_middle_horizontal.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_middle_horizontal.png');this.internal_scroll_middle_horizontal.parent_container=this.bottom_scroller;this.internal_scroll_right=new PureFW.Container(this.bottom_scroller,0,0,10,this.button_size);this.internal_scroll_right.set_x(this.internal_scroll_middle_horizontal.width+10);this.internal_scroll_right.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_right.png');this.internal_scroll_right.parent_container=this.bottom_scroller;this.add_event_handler("mouseover",(function(be){return function(rb){be.selected()}
}
)(this));this.add_event_handler("mouseout",(function(be){return function(rb){be.deselected()}
}
)(this));this.hide_all_vertical();this.hide_all_horizontal();this.update_vertical_scroller();this.update_horizontal_scroller();this.side_scroller.disable_selection();this.side_scroller_container.disable_selection();this.down_arrow_container.disable_selection();this.up_arrow_container.disable_selection();this.bottom_scroller.disable_selection();this.bottom_scroller_container.disable_selection();this.right_arrow_container.disable_selection();this.left_arrow_container.disable_selection();}
;PureFW.ScrollContainer.prototype.selected=function(){scrolling.cur_obj=this.inner_container.get_node();scrolling.max_vertical_scroll=this.inner_container.height-this.height;scrolling.max_horizontal_scroll=this.inner_container.width-this.width;scrolling.top=parseInt(this.inner_container.get_node().style.top);scrolling.while_scrolling=PureFW.ScrollContainer.while_scrolling;PureFW.ScrollContainer.active_scrolling_instance=this;this.show_all_vertical();this.show_all_horizontal();}
;PureFW.ScrollContainer.prototype.deselected=function(){this.hide_all_vertical();this.hide_all_horizontal();scrolling.cur_obj=null;scrolling.max_vertical_scroll=0;scrolling.max_horizontal_scroll=0;scrolling.top=0;}
;PureFW.ScrollContainer.prototype.update_vertical_scroller=function(){if(this.height<this.inner_container.height){this.vertical_scrolling_enabled=true;this.max_vertical_scroll=this.inner_container.height-this.height+2;this.vertical_scroller_size=(this.side_scroller_container.height)*(this.side_scroller_container.height/this.inner_container.height);if(isNaN(this.vertical_scroller_size)){this.vertical_scroller_size=0;}
if(this.vertical_scroller_size<20){this.vertical_scroller_size=20;}
this.internal_scroll_middle_vertical.set_height(this.vertical_scroller_size-20);this.internal_scroll_bottom.set_y(this.internal_scroll_middle_vertical.height+10);this.side_scroller.set_height(this.vertical_scroller_size);this.vertical_scroller_proportion=(this.side_scroller_container.height-this.vertical_scroller_size)/this.max_vertical_scroll;}
else
{this.vertical_scrolling_enabled=false;this.hide_all_vertical();this.set_inner_top(0);}
}
;PureFW.ScrollContainer.prototype.update_horizontal_scroller=function(){if(this.width<this.inner_container.width){this.horizontal_scrolling_enabled=true;this.max_horizontal_scroll=this.inner_container.width-this.width+2;this.horizontal_scroller_size=(this.bottom_scroller_container.width)*(this.bottom_scroller_container.width/this.inner_container.width);if(isNaN(this.horizontal_scroller_size)){this.horizontal_scroller_size=0;}
if(this.horizontal_scroller_size<20){this.horizontal_scroller_size=20;}
this.internal_scroll_middle_horizontal.set_width(this.horizontal_scroller_size-20);this.internal_scroll_right.set_x(this.internal_scroll_middle_horizontal.width+10);this.bottom_scroller.set_width(this.horizontal_scroller_size);this.horizontal_scroller_proportion=(this.bottom_scroller_container.width
-this.horizontal_scroller_size)/this.max_horizontal_scroll;}
else
{this.horizontal_scrolling_enabled=false;this.hide_all_horizontal();this.set_inner_left(0);}
}
;PureFW.ScrollContainer.prototype.scroll=function(bd){if(this.vertical_scrolling_enabled){if(bd<0){this.scroll_down();}
else
{this.scroll_up();}
}
else if(!this.vertical_scrolling_enabled&&this.horizontal_scrolling_enabled){if(bd<0){this.scroll_right();}
else
{this.scroll_left();}
}
}
;PureFW.ScrollContainer.prototype.scroll_up=function(ig){if((this.inner_container.position.y<=0)&&this.vertical_scrolling_enabled){var jg=this.height*this.vertical_scroll_interval;this.set_inner_top(this.inner_container.position.y+(ig||jg));if(this.inner_container.position.y>0){this.set_inner_top(0);}
}
}
;PureFW.ScrollContainer.prototype.scroll_down=function(ig){this.max_vertical_scroll=this.inner_container.height-this.height;if((-this.inner_container.position.y<=this.max_vertical_scroll)&&this.vertical_scrolling_enabled){var jg=this.height*this.vertical_scroll_interval;this.set_inner_top(this.inner_container.position.y-(ig||jg));if(-this.inner_container.position.y>this.max_vertical_scroll){this.set_inner_top(-this.max_vertical_scroll);}
}
}
;PureFW.ScrollContainer.prototype.scroll_left=function(ig){if((this.inner_container.position.x<=0)&&this.horizontal_scrolling_enabled){var jg=this.width*this.horizontal_scroll_interval;this.set_inner_left(this.inner_container.position.x+(ig||jg));if(this.inner_container.position.x>0){this.set_inner_left(0);}
}
}
;PureFW.ScrollContainer.prototype.scroll_right=function(ig){this.max_horizontal_scroll=this.inner_container.width-this.width;if((-this.inner_container.position.x<=this.max_horizontal_scroll)&&this.horizontal_scrolling_enabled){var jg=this.width*this.horizontal_scroll_interval;this.set_inner_left(this.inner_container.position.x-(ig||jg));if(-this.inner_container.position.x>this.max_horizontal_scroll){this.set_inner_left(-this.max_horizontal_scroll);}
}
}
;PureFW.ScrollContainer.prototype.set_vertical_scroll_interval=function(kg){if((!isNaN(kg)&&(kg>0&&kg<=1))){this.vertical_scroll_interval=kg;}
}
PureFW.ScrollContainer.prototype.set_horizontal_scroll_interval=function(kg){if((!isNaN(kg)&&(kg>0&&kg<=1))){this.horizontal_scroll_interval=kg;}
}
PureFW.ScrollContainer.prototype.scroll_to=function(x,y){if(x>this.max_horizontal_scroll)x=this.max_horizontal_scroll;else if(x<0)x=0;if(y>this.max_vertical_scroll)y=this.max_vertical_scroll;else if(y<0)y=0;this.set_inner_left(-x);this.set_inner_top(-y);}
PureFW.ScrollContainer.prototype.set_inner_top=function(t,lg){if(typeof(lg)=='undefined'){lg=true;}
if(t||t===0){this.inner_container.set_y(t);}
if(lg==true){this.set_scroller_vertical_position(-t);}
}
;PureFW.ScrollContainer.prototype.set_inner_left=function(l,lg){if(typeof(lg)=='undefined'){lg=true;}
if(l||l===0){this.inner_container.set_x(l);}
if(lg==true){this.set_scroller_horizontal_position(-l);}
}
;PureFW.ScrollContainer.prototype.update_inner_height=function(){this.inner_container.set_height(this.inner_container.get_height()/this.scale_factor
);this.update_vertical_scroller();}
PureFW.ScrollContainer.prototype.set_inner_height=function(h){if(h>=0){this.inner_container.set_height(h);this.update_vertical_scroller();}
}
;PureFW.ScrollContainer.prototype.set_inner_width=function(w){if(w>=0){this.inner_container.set_width(w);this.update_horizontal_scroller();}
}
;PureFW.ScrollContainer.prototype.get_button_size=function(){return(this.button_size*this.scale_factor);}
;PureFW.ScrollContainer.prototype.set_button_size=function(w){if(w>0){this.button_size=w;}
}
;PureFW.ScrollContainer.prototype.set_color=function(ye){this.scroller_color=ye;this.up_arrow_container.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowtop.png');this.down_arrow_container.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowbottom.png');this.internal_scroll_top.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_top.png');this.internal_scroll_middle_vertical.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_middle_vertical.png');this.internal_scroll_bottom.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_bottom.png');this.left_arrow_container.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowleft.png');this.right_arrow_container.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollarrowright.png');this.internal_scroll_left.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_left.png');this.internal_scroll_middle_horizontal.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_middle_horizontal.png');this.internal_scroll_right.set_bg_img('ui/elements/scrollbars/'+this.scroller_color+'/scrollbar_right.png');this.repaint();}
;PureFW.ScrollContainer.prototype.get_vertical_scroller_size=function(){return this.vertical_scroller_size*this.scale_factor;}
;PureFW.ScrollContainer.prototype.get_horizontal_scroller_size=function(){return this.horizontal_scroller_size*this.scale_factor;}
;PureFW.ScrollContainer.prototype.set_scroller_vertical_position=function(p){if(p||p===0){this.vertical_scroller_position=p;this.side_scroller.set_y(p*this.vertical_scroller_proportion);}
}
;PureFW.ScrollContainer.prototype.get_scroller_vertical_position=function(){return(this.vertical_scroller_position);}
;PureFW.ScrollContainer.prototype.set_scroller_horizontal_position=function(p){if(p||p===0){this.horizontal_scroller_position=p;this.bottom_scroller.set_x(p*this.horizontal_scroller_proportion);}
}
;PureFW.ScrollContainer.prototype.get_scroller_horizontal_position=function(){return(this.horizontal_scroller_position);}
;PureFW.ScrollContainer.prototype.show_all_vertical=function(){if(this.vertical_scrolling_enabled){this.up_arrow_container.show();this.side_scroller_container.show();this.down_arrow_container.show();}
}
;PureFW.ScrollContainer.prototype.hide_all_vertical=function(){if(this.hiding_allowed){this.up_arrow_container.hide();this.side_scroller_container.hide();this.down_arrow_container.hide();}
}
;PureFW.ScrollContainer.prototype.show_all_horizontal=function(){if(this.horizontal_scrolling_enabled){this.left_arrow_container.show();this.bottom_scroller_container.show();this.right_arrow_container.show();}
}
;PureFW.ScrollContainer.prototype.hide_all_horizontal=function(){if(this.hiding_allowed){this.left_arrow_container.hide();this.bottom_scroller_container.hide();this.right_arrow_container.hide();}
}
;PureFW.ScrollContainer.prototype._start_vertical_dragging=function(){PureFW.MouseFeatures.Dragging.start_dragging(document.getElemById(this.vertical_scroller_id));PureFW.MouseFeatures.Dragging.restrict_direction(PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_Y
);PureFW.MouseFeatures.Dragging.set_pos_limits(false,0,false,(this.side_scroller_container.height-this.vertical_scroller_size));PureFW.MouseFeatures.Dragging.add_event_handler("dragging",this.constructor.while_vertical_dragging
);PureFW.MouseFeatures.Dragging.add_event_handler("drop",this.constructor.vertical_scroller_dropped
);this.constructor.active_dragging_instance=this;this.hiding_allowed=false;}
;PureFW.ScrollContainer.prototype._start_horizontal_dragging=function(){PureFW.MouseFeatures.Dragging.start_dragging(document.getElemById(this.horizontal_scroller_id));PureFW.MouseFeatures.Dragging.restrict_direction(PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X
);PureFW.MouseFeatures.Dragging.set_pos_limits(0,false,(this.bottom_scroller_container.width-this.horizontal_scroller_size),false);PureFW.MouseFeatures.Dragging.add_event_handler("dragging",this.constructor.while_horizontal_dragging
);PureFW.MouseFeatures.Dragging.add_event_handler("drop",this.constructor.horizontal_scroller_dropped
);this.constructor.active_dragging_instance=this;this.hiding_allowed=false;}
;PureFW.ScrollContainer.prototype._stop_horizontal_dragging=function(){PureFW.ScrollContainer.active_dragging_instance=null;PureFW.MouseFeatures.Dragging.remove_event_handler("dragging",this.constructor.while_horizontal_dragging
);PureFW.MouseFeatures.Dragging.remove_event_handler("drop",this.constructor.horizontal_scroller_dropped
);this.hiding_allowed=true;this.hide_all_horizontal();}
;PureFW.ScrollContainer.prototype._stop_vertical_dragging=function(){PureFW.MouseFeatures.Dragging.remove_event_handler("dragging",this.constructor.while_vertical_dragging
);PureFW.MouseFeatures.Dragging.remove_event_handler("drop",this.constructor.vertical_scroller_dropped
);PureFW.ScrollContainer.active_dragging_instance=null;this.hiding_allowed=true;this.hide_all_vertical();}
;PureFW.ScrollContainer.prototype._calculate_vertical_value=function(mg){var fd=mg.y/this.vertical_scroller_proportion;this.set_inner_top(-fd,false);}
;PureFW.ScrollContainer.prototype._calculate_horizontal_value=function(mg){var fd=mg.x/this.horizontal_scroller_proportion;this.set_inner_left(-fd,false);}
;PureFW.ScrollContainer.prototype.set_content=function(text,yb){this.on_change(this.create_event("change"));this.inner_container.set_content(text,yb);}
;PureFW.ScrollContainer.prototype.get_content=function(){return this.get_content_node().innerHTML;}
;PureFW.ScrollContainer.prototype.get_content_node=function(){return this.inner_container.get_node();}
;PureFW.ScrollContainer.prototype.set_width=function(w){PureFW.ScrollContainer.parent.set_width.call(this,w);if((w>=this.inner_container.width||w==0)&&(this.inner_container.width!=0)){this.set_inner_width(w);}
}
;PureFW.ScrollContainer.prototype.set_height=function(h){PureFW.ScrollContainer.parent.set_height.call(this,h);if((h>=this.inner_container.height||h==0)&&(this.inner_container.height!=0)){this.set_inner_height(h);}
}
;PureFW.ScrollContainer.prototype.repaint=function(){PureFW.ScrollContainer.parent.repaint.call(this);var ng=0;var og=this.width-this.button_size;var top=0;var pg=this.height-this.button_size;this.left_arrow_container.set_y(pg);this.bottom_scroller_container.set_x(this.button_size);this.bottom_scroller_container.set_width(this.width-3*this.button_size);this.bottom_scroller_container.set_y(pg);this.right_arrow_container.set_x(og-this.button_size);this.right_arrow_container.set_y(pg);this.up_arrow_container.set_x(og);this.side_scroller_container.set_x(og);this.side_scroller_container.set_height(this.height-2*this.button_size);this.side_scroller_container.set_y(this.button_size);this.down_arrow_container.set_x(og);this.down_arrow_container.set_y(pg);this.update_vertical_scroller();this.update_horizontal_scroller();}
;PureFW.ScrollContainer.prototype.scale=function(){PureFW.ScrollContainer.parent.scale.call(this);this.set_inner_left(0);this.set_inner_top(0);}
;PureFW.ContainerAJAX=function(xd,x,y,w,h,kd,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,yd);this.url=kd||'';this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.ContainerAJAX.instances["+this.instance_num+"]";this.id="ContainerAJAX"+this.instance_num;this.bg_img_id="ContainerAJAX_bg_img"+this.instance_num;this.content_id="ContainerAJAX_cont"+this.instance_num;this.insert_into_dom_tree();if(this.url)this.load_content();this.constructor.instances[this.instance_num]=this;this.constructor.num_instances++;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.ContainerAJAX.extend(PureFW.Container);PureFW.ContainerAJAX.num_instances=0;PureFW.ContainerAJAX.instances=new Array();PureFW.ContainerAJAX.fetch_request_result=function(qg){if((typeof(qg)!='object')||qg.constructor!=Array)throw qg;if(isNaN(qg[0])){throw qg;}
var rg=parseInt(qg[0]);PureFW.ContainerAJAX.instances[rg].set_content(qg[1],true);PureFW.ContainerAJAX.instances[rg].on_load(PureFW.ContainerAJAX.instances[rg].create_event("load"));}
;PureFW.ContainerAJAX.prototype.init=function(xd,x,y,w,h,yd){PureFW.ContainerAJAX.parent.init.call(this,xd,x,y,w,h,yd);this.on_load_functions=new Array();this.bg_img=null;}
;PureFW.ContainerAJAX.prototype.set_url=function(kd){this.url=kd;this.load_content();}
;PureFW.ContainerAJAX.prototype.get_url=function(){return this.url;}
;PureFW.ContainerAJAX.prototype.load_content=function(){if(this.url){this.set_content('&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;loading...');var kd=(this.url.indexOf('?')==-1)?this.url+'?':this.url+'&';PureFW.AJAXClientServer.send_request(kd+'ajaxid='+this.instance_num,this.constructor.fetch_request_result);}
}
;PureFW.ContainerAJAX.prototype.on_load=function(rb){if(!rb){this.create_event("load")}
var n=this.on_load_functions.length;for(var i=0;i<n;i++){this.on_load_functions[i].call(this,rb);}
}
;PureFW.ContainerAJAX.prototype.add_event_handler=function(tc,hc){if(tc==="load")this.on_load_functions.push(hc);else
PureFW.ContainerAJAX.parent.add_event_handler.call(this,tc,hc);}
;PureFW.ContainerAJAX.prototype.remove_event_handler=function(tc,hc){if(tc==="load")this.on_load_functions.remove(hc);else
PureFW.ContainerAJAX.parent.remove_event_handler.call(this,tc,hc);}
;PureFW.ContainerAJAX.prototype.destroy=function(){this.constructor.instances.remove(this);PureFW.ContainerAJAX.parent.destroy.call(this);this.on_load_functions.destroy();}
PureFW.TextArea=function(xd,x,y,w,h){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.TextArea.instances["+this.instance_num+"]";this.id="TextArea"+this.instance_num;this.bg_img_id="TextArea_bg_img"+this.instance_num;this.inner_container_id="TextScrollContent"+this.instance_num;this.vertical_scroller_id="TextVertiScroller"+this.instance_num;this.horizontal_scroller_id="TextHorizonScroller"+this.instance_num;this.text_id="TextAreaContent"+this.instance_num;this.insert_into_dom_tree();this.constructor.instances[this.instance_num]=this;this.constructor.num_instances++;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.TextArea.extend(PureFW.ScrollContainer);PureFW.TextArea.num_instances=0;PureFW.TextArea.instances=new Array();PureFW.TextArea.prototype.init=function(xd,x,y,w,h){PureFW.TextArea.parent.init.call(this,xd,x,y,w,h);this.text_node=null;}
;PureFW.TextArea.prototype.insert_into_dom_tree=function(){PureFW.TextArea.parent.insert_into_dom_tree.call(this);this.text_node=document.createElement('textarea');this.text_node.style.position='absolute';this.text_node.style.width=this.width-10+'px';this.text_node.style.height=this.height-10+'px';this.text_node.style.border="0";this.text_node.style.backgroundColor='transparent';this.text_node.setAttribute("onkeyup","javascript: "+this.instance_name+".key_up(event);");this.text_node.setAttribute("onkeydown","javascript: "+this.instance_name+".key_down(event);");this.text_node.setAttribute("onchange","javascript: "+this.instance_name+".keep_one_space_ie(event);");this.text_node.id=this.text_id;this.get_content_node().appendChild(this.text_node);}
;PureFW.TextArea.prototype.set_content=function(bg){document.setElemInnerHTML(this.get_text_element(),bg);this.get_text_element().value=bg;this.keep_one_space_ie();}
;PureFW.TextArea.prototype.add_content=function(bg){document.setElemInnerHTML(this.get_text_element(),this.get_text_element().innerHTML+bg);this.get_text_element().value=bg;}
;PureFW.TextArea.prototype.get_text=function(){return this.get_text_element().value;}
;PureFW.TextArea.prototype.get_content=function(){return this.get_text();}
;PureFW.TextArea.prototype.set_name=function(name){this.text_node.setAttribute("name",name);}
PureFW.TextArea.prototype.key_up=function(rb){try
{rb=PureFW.EventUtil.formatEvent(rb);}
catch(e){}
rb.stopPropagation();if(parseInt(this.text_node.scrollHeight)>this.height){this.text_node.style.height=parseInt(this.text_node.scrollHeight)+"px";this.set_inner_height(parseInt(this.text_node.style.height));this.scroll_down(20);}
else{this.text_node.style.height=this.height+"px";this.set_inner_height(this.height);this.scroll_up(20);}
}
PureFW.TextArea.prototype.key_down=function(rb){try
{rb=PureFW.EventUtil.formatEvent(rb);}
catch(e){}
rb.stopPropagation();if(parseInt(this.text_node.scrollHeight)>this.height)this.text_node.style.height=parseInt(this.text_node.scrollHeight)+"px";else
this.text_node.style.height=this.height+"px";this.set_inner_height(parseInt(this.text_node.style.height));}
PureFW.TextArea.prototype.keep_one_space_ie=function(rb){if((this.get_content()=='')&&(navigator.userAgent.toLowerCase().indexOf("explorer")>-1)){this.set_content(' ');}
}
PureFW.TextArea.prototype.get_text_element=function(){if(document.getElemById(this.text_id)){this.text_node=document.getElemById(this.text_id);}
return this.text_node;}
;PureFW.Label=function(xd,x,y,w,h,text){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h);this.text=text||'';this.pic_url='';this.pic_h=0;this.pic_w=0;this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.Label.instances["+this.instance_num+"]";this.id="Label"+this.instance_num;this.bg_img_id="Label_bg_img"+this.instance_num;this.content_id="Label_cont"+this.instance_num;this.text_id="Label_text"+this.instance_num;this.pic_id="Label_pic"+this.instance_num;this.node=null;this.pic_node=null;this.pic_node_pic=null;this.text_node=null;this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;this.insert_into_dom_tree();PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.Label.extend(PureFW.Container);PureFW.Label.num_instances=0;PureFW.Label.instances=new Array();PureFW.Label.OVER=2;PureFW.Label.UNDER=1;PureFW.Label.LEFT=0;PureFW.Label.RIGHT=3;PureFW.Label.prototype.insert_into_dom_tree=function(){PureFW.Label.parent.insert_into_dom_tree.call(this);this.text_node=document.createElement('div');this.text_node.innerHTML=this.text;this.node.appendChild(this.text_node);}
;PureFW.Label.prototype.set_pic=function(re,w,h,gf){if(this.pic_node)this.node.removeChild(this.pic_node);if(this.text_node)this.node.removeChild(this.text_node);this.pic_w=w||20;this.pic_h=h||20;this.pic_url=re;this.pic_node_pic=document.createElement('img');this.pic_node_pic.style.width=this.get_pic_width()+"px";this.pic_node_pic.style.height=this.get_pic_height()+"px";this.pic_node_pic.src=this.add_pic_path(this.pic_url);this.pic_node_pic.id=this.pic_id;switch(gf){case PureFW.Label.OVER:this.pic_node=document.createElement('center');this.node.appendChild(this.pic_node);this.text_node=document.createElement('center');this.text_node.id=this.text_id;this.node.appendChild(this.text_node);break;case PureFW.Label.UNDER:this.text_node=document.createElement('center');this.text_node.id=this.text_id;this.node.appendChild(this.text_node);this.pic_node=document.createElement('center');this.node.appendChild(this.pic_node);break;case PureFW.Label.RIGHT:var sg=document.createElement('table');sg.setAttribute("border",0);sg.setAttribute("cellspacing",0);sg.setAttribute("cellpadding",2);var tg=document.createElement('tr');sg.appendChild(tg);this.text_node=document.createElement('td');this.text_node.setAttribute("valign","middle");this.text_node.id=this.text_id;tg.appendChild(this.text_node);this.pic_node=document.createElement('td');this.pic_node.setAttribute("valign","middle");tg.appendChild(this.pic_node);this.node.appendChild(sg);break;default:var sg=document.createElement('table');sg.setAttribute("border",0);sg.setAttribute("cellspacing",0);sg.setAttribute("cellpadding",2);var tg=document.createElement('tr');sg.appendChild(tg);this.pic_node=document.createElement('td');this.pic_node.setAttribute("valign","middle");tg.appendChild(this.pic_node);this.text_node=document.createElement('td');this.text_node.setAttribute("valign","middle");this.text_node.id=this.text_id;tg.appendChild(this.text_node);this.node.appendChild(sg);}
this.text_node.innerHTML=this.text;this.pic_node.appendChild(this.pic_node_pic);}
;PureFW.Label.prototype.set_text=function(text){this.text=text;if(this.text_node)this.text_node=document.getElemById(this.text_id);document.setElemInnerHTML(this.text_node,text);}
;PureFW.Label.prototype.set_font_color=function(ye){if(this.text_node)this.text_node=document.getElemById(this.text_id);document.getElemById(this.text_id).style.color=ye;}
PureFW.Label.prototype.set_content=function(text){this.set_text(text);}
PureFW.Label.prototype.set_title=function(ug){document.getElemById(this.id).title=ug;}
;PureFW.Label.prototype.get_pic_width=function(){return this.pic_w*this.scale_factor;}
;PureFW.Label.prototype.get_pic_height=function(){return this.pic_h*this.scale_factor;}
;PureFW.Label.prototype.repaint=function(){this.node=this.get_node();this.pic_node=document.getElemById(this.pic_node_id);if(this.node){if(this.width)this.node.style.width=this.get_width()+"px";if(this.height)this.node.style.height=this.get_height()+"px";this.node.style.left=this.get_x()+"px";this.node.style.top=this.get_y()+"px";if(this.pic_node_pic){this.pic_node_pic=document.getElementById(this.pic_id);this.pic_node_pic.style.width=this.get_pic_width()+"px";this.pic_node_pic.style.height=this.get_pic_height()+"px";}
}
}
;PureFW.FilmAnimator=function(xd,x,y,w,h,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,yd);this.width=w||40;this.height=h||40;this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.FilmAnimator.instances["+this.instance_num+"]";this.id="FilmAnimator"+this.instance_num;this.content_id="FilmAnimatorCont"+this.instance_num;this.bg_id="FilmAnimatorBg"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.FilmAnimator.extend(PureFW.Container);PureFW.FilmAnimator.num_instances=0;PureFW.FilmAnimator.instances=new Array();PureFW.FilmAnimator.prototype.init=function(xd,x,y,w,h,yd){PureFW.FilmAnimator.parent.init.call(this,xd,x,y,w,h,yd);this.number_of_frames=null;this.current_frame=0;this.interval=null;this.running_film=false;this.run_on_show=false;this.inner_frame=null;this.frame_range_min=new Array();this.frame_range_max=new Array();this.reverse=false;this.yoyo=false;this.strip=0;this.strips=1;this.on_loop_functions=new Array();this.frame_delay=100;}
;PureFW.FilmAnimator.prototype.insert_into_dom_tree=function(){PureFW.FilmAnimator.parent.insert_into_dom_tree.call(this);this.inner_frame=this.add_widget(PureFW.Image,0,0,this.width,this.height,'',this.no_scale
);this.inner_frame.repaint=(function(vg){return function(rb){PureFW.Image.parent.repaint.call(this);var frames=vg.number_of_frames+1;var af=Math.floor(this.get_height()/frames);this.get_node().style.height=(af*frames)+'px';vg.get_node().style.height=af+'px';}
}
)(this);this.parent_node.appendChild(this.node);}
;PureFW.FilmAnimator.prototype.set_image=function(hd,wg,xg){this.strips=xg||1;this.strip=0;this.number_of_frames=wg-1;this.frame_range_min=new Array();this.frame_range_max=new Array();for(var i=0;i<this.strips;i++){this.frame_range_min[i]=0;this.frame_range_max[i]=this.number_of_frames;}
this.inner_frame.set_pic_url(hd);this.inner_frame.set_height(this.height*wg);this.inner_frame.set_width(this.width*xg);if(this.is_running()){this.stop_animation();this.start_animation();}
}
;PureFW.FilmAnimator.prototype.is_running=function(){return this.running_film;}
PureFW.FilmAnimator.prototype.set_strip=function(yg){this.stop_animation();if(yg>=this.strips)this.strip=this.strips-1;else if(yg<0)this.strip=0;else
this.strip=yg;if(this.reverse)this.current_frame=this.frame_range_max[this.strip];else
this.current_frame=this.frame_range_min[this.strip];this.inner_frame.set_x((-this.strip)*this.width);this.inner_frame.set_y(0);this.start_animation();}
PureFW.FilmAnimator.prototype.set_frame_range=function(zg,eg,yg){yg=yg||0;this.frame_range_min[yg]=(zg<0)?0:zg;this.frame_range_max[yg]=(eg>this.number_of_frames)?this.number_of_frames:eg;this.current_frame=zg;}
PureFW.FilmAnimator.prototype.set_frame_delay=function(kg){this.stop_animation();this.frame_delay=kg;this.start_animation();}
PureFW.FilmAnimator.prototype.animate=function(){if(this.current_frame>this.frame_range_max[this.strip]){if(this.yoyo){this.reverse=!this.reverse;this.current_frame=this.frame_range_max[this.strip];}
else
this.current_frame=this.frame_range_min[this.strip];this.on_loop();}
else if(this.current_frame<this.frame_range_min[this.strip]){if(this.yoyo){this.reverse=!this.reverse;this.current_frame=this.frame_range_min[this.strip];}
else
this.current_frame=this.frame_range_max[this.strip];this.on_loop();}
if(!this.inner_frame.get_node())this.stop_animation();this.inner_frame.get_node().style.top=((-this.current_frame)*Math.floor(this.inner_frame.get_height()/(this.number_of_frames+1)))+'px';if(this.reverse)this.current_frame--;else
this.current_frame++;}
;PureFW.FilmAnimator.prototype.toggle_animation=function(){if(this.is_running())this.stop_animation();else
this.start_animation();}
PureFW.FilmAnimator.prototype.start_animation=function(){if(this.is_running())return;if((this.frame_range_max[this.strip]-this.frame_range_min[this.strip])<1){return;}
this.running_film=true;this.interval=PureFW.Timeout.set_interval((function(be){return function(){try{be.animate();}
catch(e){be.stop_animation();}
}
}
)(this),this.frame_delay
);}
;PureFW.FilmAnimator.prototype.stop_animation=function(){if(!this.is_running())return;this.running_film=false;PureFW.Timeout.clear_interval(this.interval);this.interval=null;}
;PureFW.FilmAnimator.prototype.hide=function(){PureFW.FilmAnimator.parent.hide.call(this);if(this.is_running()){this.run_on_show=true;this.stop_animation();}
}
;PureFW.FilmAnimator.prototype.show=function(){PureFW.FilmAnimator.parent.show.call(this);if(this.run_on_show)this.start_animation();this.run_on_show=false;}
;PureFW.FilmAnimator.prototype.destroy=function(){this.stop_animation();PureFW.FilmAnimator.parent.destroy.call(this);}
PureFW.FilmAnimator.prototype.add_event_handler=function(tc,hc){if(tc==="loop")this.on_loop_functions.push(hc);else
PureFW.FilmAnimator.parent.add_event_handler.call(this,tc,hc);}
;PureFW.FilmAnimator.prototype.remove_event_handler=function(tc,hc){if(tc==="loop")this.on_loop_functions.remove(hc);else
PureFW.FilmAnimator.parent.remove_event_handler.call(this,tc,hc);}
;PureFW.FilmAnimator.prototype.on_loop=function(rb){if(!rb)rb=this.create_event("loop");var n=this.on_loop_functions.length;for(var i=0;i<n;i++){this.on_loop_functions[i].call(this,rb);}
}
;PureFW.FilmAnimator.prototype.set_width=function(w,zd,$d,_d){PureFW.FilmAnimator.parent.set_width.call(this,w,zd,$d,_d);this.inner_frame.set_width(w,zd,$d,_d);}
;PureFW.FilmAnimator.prototype.set_height=function(h,zd,$d,_d){PureFW.FilmAnimator.parent.set_height.call(this,h,zd,$d,_d);this.inner_frame.set_height(h*this.number_of_frames+1,zd,$d,_d);}
;PureFW.FilmAnimator.prototype.repaint=function(){PureFW.FilmAnimator.parent.repaint.call(this);this.get_node().style.height=(Math.floor(this.inner_frame.get_height()/(this.number_of_frames+1)))+'px';}
PureFW.WindowAJAX=function(xd,x,y,w,h,kd,ug){if(typeof(this.parent_node)!='undefined'){this.init(xd,x,y,w,h);this.title=ug||'';this.bg_img=null;this.url=kd||'';this.frame_size=new PureFW.Point(15,0);this.button_bar_width=32;this.title_bar_height=32;this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.WindowAJAX.instances["+this.instance_num+"]";this.id="WindowAJAX"+this.instance_num;this.bg_img_id="WindowAJAX_img"+this.instance_num;this.content_id="WindowAJAX_cont"+this.instance_num;this.body=null;this.title_bar=null;this.button_bar=null;if(this.parent_node)this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.WindowAJAX.extend(PureFW.Container);PureFW.WindowAJAX.num_instances=0;PureFW.WindowAJAX.instances=new Array();PureFW.WindowAJAX.prototype.set_url=function(kd){this.body.set_url(kd);}
;PureFW.WindowAJAX.prototype.get_url=function(){return this.body.get_url();}
PureFW.WindowAJAX.prototype.set_title=function(vd){this.title=vd;document.setElemInnerHTML(document.getElemById(this.title_bar.get_id()),vd);}
;PureFW.WindowAJAX.prototype.repaint=function(){PureFW.WindowAJAX.parent.repaint.call(this);try{this.body.repaint();this.title_bar.repaint();this.button_bar.repaint();}
catch(e){}
}
PureFW.WindowAJAX.prototype.insert_into_dom_tree=function(){PureFW.WindowAJAX.parent.insert_into_dom_tree.call(this);this.set_z_index(10);var $g=document.getElemById(this.id);this.title_bar=new PureFW.Container($g,this.frame_size.x,this.frame_size.y,this.width-(this.frame_size.x<<1)-this.button_bar_width,this.title_bar_height
);this.title_bar.set_content(this.title);this.title_bar.set_font_size(1.1);this.title_bar.set_font_weight('bold');this.title_bar.set_padding_top(((this.title_bar_height-14)>>1)-1);this.title_bar.set_padding_left(this.frame_size.x+3);this.button_bar=new PureFW.Container($g,this.width-4-this.button_bar_width,1,this.button_bar_width,this.title_bar_height
);this.button_bar.set_bg_img('../pix/ui/elements/window/close_window.png');this.destroy_caller=(function _g(ah){return function(rb){PureFW.WindowAJAX.instances[ah].destroy();}
}
)(this.instance_num);this.button_bar.add_event_handler('click',this.destroy_caller);this.body=new PureFW.ContainerAJAX($g,this.frame_size.x,this.frame_size.y+this.title_bar_height,this.width-(this.frame_size.x<<1),this.height-(this.frame_size.y<<1)-this.title_bar_height,this.url
);}
;PureFW.WindowAJAX.prototype.set_content=function(bg,yb){this.body.set_content(bg,yb);}
PureFW.WindowAJAX.prototype.disable_close=function(){this.button_bar.remove_event_handler('click',this.destroy_caller);this.button_bar.hide();}
PureFW.WindowAJAX.prototype.destroy=function(){this.title_bar.destroy();this.button_bar.destroy();this.body.destroy();PureFW.WindowAJAX.parent.destroy.call(this);}
PureFW.WindowAJAX.prototype.add_event_handler=function(tc,hc){if(tc==="change"){this.body.add_event_handler(tc,hc);}
else{PureFW.WindowAJAX.parent.add_event_handler.call(this,tc,hc);}
}
PureFW.Icon=function(xd,x,y,w,h,bh,ue){if(typeof(xd)!=='undefined'){this.init();this.parent_node=xd||document.body;this.position=new PureFW.Point(x||0,y||0);this.width=w||48;this.height=h||48;this.color=ue||'white';this.pic_url=bh||'';this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.Icon.instances["+this.instance_num+"]";this.id='Icon'+this.constructor.num_instances;this.node=null;this.subtitle='';this.tooltip='';this.overicons='';this.bg_img='';this.subtitle_id='Icon_subtitle'+this.instance_num;this.subtitle_node=null;this.overicons_id='Icon_overlays'+this.instance_num;this.overicons_node=null;this.img_id='Icon_img'+this.instance_num;this.img_node=null;this.bg_img_node=null;this.bg_img_id='Icon_bg_img'+this.instance_num;this.set_color(this.color);this.constructor.instances[this.instance_num]=this;this.constructor.num_instances++;this.insert_into_dom_tree();PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.Icon.extend(PureFW.Widget);PureFW.Icon.num_instances=0;PureFW.Icon.instances=new Array();PureFW.Icon.prototype.insert_into_dom_tree=function(){PureFW.Icon.parent.insert_into_dom_tree.call(this);this.node=document.createElement('div');this.node.style.width=this.get_width()+'px';this.node.style.height=this.get_height()+'px';this.node.style.position='absolute';this.node.style.left=this.get_x()+'px';this.node.style.top=this.get_y()+'px';this.node.style.cursor='pointer';this.node.style.color=this.color;this.node.style.overflow='hidden';this.node.id=this.id;this.bg_img_node=document.createElement('img');this.bg_img_node.style.width="100%";this.bg_img_node.style.height="100%";this.bg_img_node.style.display='block';this.bg_img_node.style.zIndex=0;this.bg_img_node.style.position='absolute';this.bg_img_node.style.left=0;this.bg_img_node.style.top=0;if(this.bg_img){this.bg_img_node.src=this.add_pic_path(this.bg_img);}
this.bg_img_node.id=this.bg_img_id;this.img_node=document.createElement('img');this.img_node.src=this.add_pic_path(this.pic_url);this.img_node.style.width="100%";this.img_node.style.height="100%";this.img_node.style.position='absolute';this.img_node.style.left=0;this.img_node.style.top=0;this.img_node.style.display='block';this.img_node.style.zIndex=1;this.img_node.style.cursor='pointer';this.img_node.id=this.img_id;this.subtitle_node=document.createElement('div');this.subtitle_node.style.textAlign='center';this.subtitle_node.style.cursor='pointer';this.subtitle_node.style.width="100%";this.subtitle_node.style.height=16*this.scale_factor+'px';this.subtitle_node.style.position='absolute';this.subtitle_node.style.left=0;this.subtitle_node.style.top=(this.get_height()-16*this.scale_factor)+'px';this.subtitle_node.style.zIndex=2;this.subtitle_node.cursor='pointer';this.subtitle_node.innerHTML=this.subtitle;this.subtitle_node.id=this.subtitle_id;this.overicons_node=document.createElement('div');this.overicons_node.display='none';if(this.tooltip)this.node.title=this.tooltip;this.parent_node.appendChild(this.node);this.node.appendChild(this.bg_img_node);this.node.appendChild(this.img_node);this.node.appendChild(this.overicons_node);this.node.appendChild(this.subtitle_node);}
;PureFW.Icon.prototype.set_subtitle=function(ch){this.subtitle=ch;this.subtitle_node=document.getElemById(this.subtitle_id);if(this.subtitle_node){document.setElemInnerHTML(this.subtitle_node,this.subtitle);if(!this.subtitle||this.subtitle==''){this.subtitle_node.style.display='none';this.img_node.style.height=this.get_height()+'px';}
else{this.subtitle_node.style.display='block';this.img_node.height=(this.get_width()-parseInt(this.subtitle_node.style.height))+'px';}
}
}
;PureFW.Icon.prototype.set_color=function(ye){this.color=ye;if(this.width==this.height)this.set_bg_img('/ui/icons/backgrounds/bg_'+this.color+'_'+this.width+'.png');else
this.set_bg_img('/ui/icons/backgrounds/bg_'+this.color+'_'+this.width+'x'+this.height+'.png');}
;PureFW.Icon.prototype.get_color=function(){return this.color;}
;PureFW.Icon.prototype.set_tooltip=function(vd){this.tooltip=vd;if(xb=this.get_node())xb.setAttribute("title",this.tooltip);}
;PureFW.Icon.prototype.set_icon_overlay_imgs=function(dh){this.overicons='';for(var i=0;i<this.img_url_arr.length;i++){this.overicons+='<img src="'+this.add_pic_path(this.img_url_arr[i])+'" width="16" height="16" />';}
if(this.overicons_node){document.setElemInnerHTML(this.overicons_node,this.overicons);if(!this.overicons||this.overicons==''){this.overicons_node.style.display='none';this.img_node.style.width=this.get_width()+'px';}
else{this.overicons_node.style.display='block';this.img_node.style.width=(this.get_width()-parseInt(this.overicons_node.style.width))+'px';}
}
}
;PureFW.Icon.prototype.add_icon_overlay_img=function(eh){this.overicons+='<img src="'+this.add_pic_path(this.img_url)+'" width="16" height="16" />';if(this.overicons_node){document.setElemInnerHTML(this.overicons_node,this.overicons_node.innerHTML+this.overicons);this.overicons_node.style.display='block';this.img_node.style.width=(this.get_width()-parseInt(this.overicons_node.style.width))+'px';}
}
;PureFW.Icon.prototype.set_bg_img=function(ub){this.bg_img=ub;if(this.bg_img_node=document.getElemById(this.bg_img_id)){this.bg_img_node.src=this.add_pic_path(this.bg_img);}
}
;PureFW.Icon.prototype.repaint=function(){this.node=document.getElemById(this.id);this.subtitle_node=document.getElemById(this.subtitle_id);this.img_node=document.getElemById(this.img_id);if(this.node){this.node.style.width=this.get_width()+'px';this.node.style.height=this.get_height()+'px';this.node.style.top=this.get_y()+'px';this.node.style.left=this.get_x()+'px';this.subtitle_node.style.top=(this.get_height()-16*this.scale_factor)+'px';this.img_node.src=this.add_pic_path(this.pic_url);}
this.set_color(this.get_color());}
;PureFW.Icon.prototype.get_horizontal_space=function(){return(this.width+10)*this.scale_factor;}
PureFW.Slider=function(xd,x,y,w,h,zg,eg,fd){this.on_value_change_functions=new Array();this.on_slide_functions=new Array();if(typeof(xd)!='undefined'){this.init();this.parent_node=xd||document.body;this.width=w||100;this.height=h||19;this.position=new PureFW.Point(x||0,y||0);this.min=zg||0;this.max=eg||1000;this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.Slider.instances["+this.instance_num+"]";this.node=null;this.id='Slider'+this.instance_num;this.slider_button_id='SliderB'+this.instance_num;this.value=fd||0;this.slider_pos=0;this.old_value=this.value;this.left_arrow_container=null;this.left_arrow_url='ui/elements/sliders/arrow_left.png';this.bg_img_left_node=null;this.bg_img_left_url='ui/elements/sliders/bgl.png';this.bg_img_left_id='SliderBGL'+this.instance_num;this.bg_img_node=null;this.bg_img_url='ui/elements/sliders/bg.png';this.bg_img_id='SliderBG'+this.instance_num;this.bg_img_right_node=null;this.bg_img_right_url='ui/elements/sliders/bgr.png';this.bg_img_right_id='SliderBGR'+this.instance_num;this.right_arrow_container=null;this.right_arrow_url='ui/elements/sliders/arrow_right.png';this.slider_button_container=null;this.slider_button_url='ui/elements/sliders/button_19x19.png';this.insert_into_dom_tree();this._calculate_slider_pos();this.constructor.instances[this.instance_num]=this;this.constructor.num_instances++;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.Slider.extend(PureFW.Widget);PureFW.Slider.num_instances=0;PureFW.Slider.instances=new Array();PureFW.Slider.active_dragging_instance=null;PureFW.Slider.while_dragging=function(rb){PureFW.Slider.active_dragging_instance.slider_pos=rb.detail.x;PureFW.Slider.active_dragging_instance._calculate_value();PureFW.Slider.active_dragging_instance.on_slide(PureFW.Slider.active_dragging_instance.create_event("slide",PureFW.Slider.active_dragging_instance.get_value()));}
;PureFW.Slider.left_arrow_clicked=function(rb){this.parent_container.set_value(this.parent_container.value-1);}
PureFW.Slider.right_arrow_clicked=function(rb){this.parent_container.set_value(this.parent_container.value+1);}
PureFW.Slider.stop_dragging_wrapper=function(rb){PureFW.Slider.active_dragging_instance._stop_dragging(rb.detail);}
;PureFW.Slider.prototype.insert_into_dom_tree=function(){this.node=document.createElement('div');this.node.style.width=this.get_width()+"px";this.node.style.height=this.get_height()+"px";this.node.style.overflow='hidden';this.node.style.position='absolute';this.node.style.left=this.get_x()+"px";this.node.style.top=this.get_y()+"px";this.node.id=this.id;this.left_arrow_container=new PureFW.Container(this.node,0,0,this.height,this.height);this.left_arrow_container.set_bg_img(this.left_arrow_url);this.left_arrow_container.add_event_handler("click",PureFW.Slider.left_arrow_clicked);this.left_arrow_container.set_z_index(2);this.left_arrow_container.parent_container=this;this.bg_img_left_node=document.createElement('img');this.bg_img_left_node.src=this.add_pic_path(this.bg_img_left_url);this.bg_img_left_node.style.position='absolute';this.bg_img_left_node.style.width=4*this.scale_factor+"px";this.bg_img_left_node.style.height="100%";this.bg_img_left_node.style.left=this.get_height()+"px";this.bg_img_left_node.setAttribute("onmousedown","javascript: return false;");this.bg_img_left_node.id=this.bg_img_left_id;this.bg_img_node=document.createElement('img');this.bg_img_node.src=this.add_pic_path(this.bg_img_url);this.bg_img_node.style.height="100%";this.bg_img_node.style.width=(this.get_width()-8*this.scale_factor
-(this.get_height()<<1))+"px";this.bg_img_node.style.position='absolute';this.bg_img_node.style.left=this.get_height()+4*this.scale_factor+"px";this.bg_img_node.setAttribute("onmousedown","javascript: return false;");this.bg_img_node.id=this.bg_img_id;this.bg_img_right_node=document.createElement('img');this.bg_img_right_node.src=this.add_pic_path(this.bg_img_right_url);this.bg_img_right_node.style.position='absolute';this.bg_img_right_node.style.width=4*this.scale_factor+"px";this.bg_img_right_node.style.height="100%";this.bg_img_right_node.style.left=(this.get_width()-this.get_height()-4*this.scale_factor)+"px";this.bg_img_right_node.setAttribute("onmousedown","javascript: return false;");this.bg_img_right_node.id=this.bg_img_right_id;this.right_arrow_container=new PureFW.Container(this.node,0,0,this.height,this.height);this.right_arrow_container.set_bg_img(this.right_arrow_url);this.right_arrow_container.add_event_handler("click",PureFW.Slider.right_arrow_clicked);this.right_arrow_container.set_z_index(2);this.right_arrow_container.set_x(this.width-this.height);this.right_arrow_container.parent_container=this;this.slider_button_container=new PureFW.Container(this.node,0,0,this.height,this.height);this.slider_button_container.set_id(this.slider_button_id);this.slider_button_container.set_bg_img(this.slider_button_url);this.slider_button_container.set_content(this.get_value());this.slider_button_container.add_event_handler("mousedown",(function(be){return function(rb){be._start_dragging();}
}
)(this));this.slider_button_container.get_node().style.cursor="pointer";this.slider_button_container.set_z_index(3);this.slider_button_container.parent_container=this;this.slider_button_container.get_node().style.textAlign='center';this.parent_node.appendChild(this.node);this.node.appendChild(this.bg_img_left_node);this.node.appendChild(this.bg_img_node);this.node.appendChild(this.bg_img_right_node);this.disable_selection();PureFW.Widget.disable_selection(this.bg_img_node);PureFW.Widget.disable_selection(this.bg_img_right_node);PureFW.Widget.disable_selection(this.bg_img_left_node);}
;PureFW.Slider.prototype.set_min=function(zg){if(zg>=this.max)this.set_max(zg+1);if(this.value<zg)this.set_value(zg);this.min=zg;this._calculate_value();}
;PureFW.Slider.prototype.set_max=function(eg){if(eg<this.min)this.set_min(eg-1);this.max=eg;this._calculate_value();}
;PureFW.Slider.prototype.set_value=function(fd){this.old_value=this.value;this.value=fd;this._calculate_slider_pos();this.on_value_change(this.create_event("value_change",fd));}
;PureFW.Slider.prototype.add_event_handler=function(tc,hc){if(tc==="value_change"){this.on_value_change_functions.push(hc);}
else if(tc==="slide"){this.on_slide_functions.push(hc);}
else{PureFW.Slider.parent.add_event_handler.call(this,tc,hc);}
}
;PureFW.Slider.prototype.remove_event_handler=function(tc,hc){if(tc==="value_change"){this.on_value_change_functions.remove(hc);}
else if(tc==="slide"){this.on_slide_functions.remove(hc);}
else{PureFW.Slider.parent.remove_event_handler.call(this,tc,hc);}
}
;PureFW.Slider.prototype.on_value_change=function(rb){if(!rb)rb=this.create_event("value_change");var n=this.on_value_change_functions.length;for(var i=0;i<n;i++)this.on_value_change_functions[i](rb);}
;PureFW.Slider.prototype.on_slide=function(rb){if(!rb)rb=this.create_event("slide");var n=this.on_slide_functions.length;for(var i=0;i<n;i++)this.on_slide_functions[i](rb);}
;PureFW.Slider.prototype.get_value=function(){return this.value;}
;PureFW.Slider.prototype.get_old_value=function(){return this.old_value;}
;PureFW.Slider.prototype.repaint=function(){try{PureFW.Slider.parent.repaint.call(this);this.bg_img_left_node=document.getElemById(this.bg_img_left_id);this.bg_img_left_node.src=this.add_pic_path(this.bg_img_left_url);this.bg_img_left_node.style.width=4*this.scale_factor+"px";this.bg_img_left_node.style.left=this.get_height()+"px";this.bg_img_node=document.getElemById(this.bg_img_id);this.bg_img_node.src=this.add_pic_path(this.bg_img_url);this.bg_img_node.style.width=(this.get_width()-8*this.scale_factor
-(this.get_height()<<1))+"px";this.bg_img_node.style.left=this.get_height()+4*this.scale_factor
+"px";this.bg_img_right_node=document.getElemById(this.bg_img_right_id);this.bg_img_right_node.src=this.add_pic_path(this.bg_img_right_url);this.bg_img_right_node.style.width=4*this.scale_factor+"px";this.bg_img_right_node.style.left=(this.get_width()-this.get_height()-4*this.scale_factor)+"px";this._calculate_slider_pos();}
catch(e){delete this;}
}
;PureFW.Slider.prototype._start_dragging=function(){PureFW.MouseFeatures.Dragging.start_dragging(document.getElemById(this.slider_button_id));PureFW.MouseFeatures.Dragging.restrict_direction(PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X
);PureFW.MouseFeatures.Dragging.set_pos_limits(this.get_height()-this.scale_factor,false,this.get_width()-19*this.scale_factor-this.get_height()-this.scale_factor,false
);PureFW.MouseFeatures.Dragging.add_event_handler("dragging",this.constructor.while_dragging
);PureFW.MouseFeatures.Dragging.add_event_handler("drop",this.constructor.stop_dragging_wrapper
);this.constructor.active_dragging_instance=this;}
;PureFW.Slider.prototype._stop_dragging=function(){this._calculate_value();this.on_value_change(this.create_event("value_change",this.value));}
;PureFW.Slider.prototype._calculate_value=function(){this.old_value=this.value;this.value=Math.round((this.slider_pos+this.min-this.height)/(this.width-19-(this.height<<1))*(this.max-this.min)/this.scale_factor
);this.slider_button_container.set_content(this.value);}
;PureFW.Slider.prototype._calculate_slider_pos=function(){this.slider_pos=this.height+Math.round(this.value/(this.max-this.min)*(this.width-19-(this.height<<1))-this.min);this.slider_button_container.set_x(this.slider_pos);}
;PureFW.Scrollbar=function(xd,x,y,w,l,ad){if(typeof(xd)!=='undefined'){this.init();this.parent_node=xd||document.body;this.position=new PureFW.Point(x||0,y||0);this.width=w||10;this.height=l||100;this.vertical=ad?!!ad:false;this.arrow1_pic_url='';this.arrow2_pic_url='';this.inst_num=this.constructor.num_instances;this.id="Scrollbar"+this.inst_num;this.node=null;this.arrow1_node=this.arrow2_node=this.bar_node=null;this.document_size=false;this.document_pos=0;this.scroll_step=5;this.scroll_interval=20;this.scroll_plus_interval_handler=null;this.scroll_minus_interval_handler=null;this.scroll_ani_step=8;this.scroll_ani_interval=0;this.scroll_ani_plus_interval_handler=null;this.scroll_ani_minus_interval_handler=null;this.scroll_ani_steps_left=0;this.scroll_ani_is_running=false;this.on_scroll_functions=new Array();this.constructor.instances[this.inst_num]=this;this.constructor.num_instances++;this.insert_into_dom_tree();PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.Scrollbar.extend(PureFW.Widget);PureFW.Scrollbar.num_instances=0;PureFW.Scrollbar.instances=new Array();PureFW.Scrollbar.prototype.insert_into_dom_tree=function(){PureFW.Scrollbar.parent.insert_into_dom_tree.call(this);this.node=document.createElement('div');this.node.style.position='absolute';this.node.style.left=this.get_x()+"px";this.node.style.top=this.get_y()+"px";this.node.id=this.id;this.arrow1_node=document.createElement('div');this.arrow1_node.setAttribute("onmousedown","javascript: PureFW.Scrollbar."+"instances["+this.inst_num+"]._scroll_minus_start();");this.arrow1_node.setAttribute("onmouseup","javascript: PureFW.Scrollbar."+"instances["+this.inst_num+"]._scroll_minus_stop();");this.arrow1_node.setAttribute("onmouseout","javascript: PureFW.Scrollbar."+"instances["+this.inst_num+"]._scroll_minus_stop();");this.arrow1_node.style.width=this.get_width()+"px";this.arrow1_node.style.height=this.get_width()+"px";this.arrow1_node.style.backgroundRepeat='no-repeat';this.arrow1_node.style.backgroundPosition='center';this.arrow1_node.style.position='absolute';this.arrow1_node.style.cursor='pointer';this.bar_node=document.createElement('div');this.bar_node.style.position='absolute';this.arrow2_node=document.createElement('div');this.arrow2_node.setAttribute("onmousedown","javascript: PureFW.Scrollbar."+"instances["+this.inst_num+"]._scroll_plus_start();");this.arrow2_node.setAttribute("onmouseup","javascript: PureFW.Scrollbar."+"instances["+this.inst_num+"]._scroll_plus_stop();");this.arrow2_node.setAttribute("onmouseout","javascript: PureFW.Scrollbar."+"instances["+this.inst_num+"]._scroll_plus_stop();");this.arrow2_node.style.width=this.get_width()+"px";this.arrow2_node.style.height=this.get_width()+"px";this.arrow2_node.style.backgroundRepeat='no-repeat';this.arrow2_node.style.backgroundPosition='center';this.arrow2_node.style.position='absolute';this.arrow2_node.style.cursor='pointer';if(this.vertical){this.node.style.width=this.get_width()+"px";this.node.style.height=this.get_height()+"px";this.arrow1_node.innerHTML='⇑';this.arrow1_node.style.left="0px";this.arrow1_node.style.top="0px";this.bar_node.style.width=this.get_width()+"px";this.bar_node.style.height=this.get_height()-(this.get_width()<<1)+"px";this.bar_node.style.left="0px";this.bar_node.style.top=this.get_width()+"px";this.arrow2_node.innerHTML='⇓';this.arrow2_node.style.left="0px";this.arrow2_node.style.top=(this.get_height()-this.get_width())+"px";}
else{this.node.style.width=this.get_height()+"px";this.node.style.height=this.get_width()+"px";this.arrow1_node.innerHTML='⇐';this.arrow1_node.style.left="0px";this.arrow1_node.style.top="0px";this.bar_node.style.width=this.get_height()-(this.get_width()<<1)+"px";this.bar_node.style.height=this.get_width()+"px";this.bar_node.style.left=this.get_width()+"px";this.bar_node.style.top="0px";this.arrow2_node.innerHTML='⇒';this.arrow2_node.style.left=(this.get_height()-this.get_width())+"px";this.arrow2_node.style.top="0px";}
this.node.appendChild(this.arrow1_node);this.node.appendChild(this.bar_node);this.node.appendChild(this.arrow2_node);this.parent_node.appendChild(this.node);}
;PureFW.Scrollbar.prototype.set_arrow_pics=function(fh,gh){this.arrow1_pic_url=fh;this.arrow2_pic_url=gh;this.repaint();}
;PureFW.Scrollbar.prototype.set_scroll_step=function(hh){this.scroll_step=hh;}
;PureFW.Scrollbar.prototype.set_scroll_interval=function(ih){this.scroll_interval=ih;}
;PureFW.Scrollbar.prototype.set_animate_scroll=function(b){this.scroll_ani_interval=(b)?20:0;}
;PureFW.Scrollbar.prototype.set_document_size=function(te){this.document_size=te;}
;PureFW.Scrollbar.prototype.repaint=function(){this.node.style.left=this.get_x()+"px";this.node.style.top=this.get_y()+"px";this.arrow1_node.style.width=this.get_width()+"px";this.arrow1_node.style.height=this.get_width()+"px";if(this.arrow1_pic_url)this.arrow1_node.style.backgroundImage='url('+this.add_pic_path(this.arrow1_pic_url)+')';else{this.arrow1_node.style.backgroundImage='';this.arrow1_node.innerHTML=(this.vertical)?'⇑':'⇐';}
this.arrow2_node.style.width=this.get_width()+"px";this.arrow2_node.style.height=this.get_width()+"px";if(this.arrow2_pic_url)this.arrow1_node.style.backgroundImage='url('+this.add_pic_path(this.arrow2_pic_url)+')';else{this.arrow2_node.style.backgroundImage='';this.arrow2_node.innerHTML=(this.vertical)?'⇓':'⇒';}
if(this.vertical){this.node.style.width=this.get_width()+"px";this.node.style.height=this.get_height()+"px";this.arrow1_node.style.left="0px";this.arrow1_node.style.top="0px";this.bar_node.style.width=this.get_width()+"px";this.bar_node.style.height=this.get_height()-(this.get_width()<<1)+"px";this.bar_node.style.left="0px";this.bar_node.style.top=this.get_width()+"px";this.arrow2_node.style.left="0px";this.arrow2_node.style.top=(this.get_height()-this.get_width())+"px";}
else{this.node.style.width=this.get_height()+"px";this.node.style.height=this.get_width()+"px";this.arrow1_node.style.left="0px";this.arrow1_node.style.top="0px";this.bar_node.style.width=this.get_height()-(this.get_width()<<1)+"px";this.bar_node.style.height=this.get_width()+"px";this.bar_node.style.left=this.get_width()+"px";this.bar_node.style.top="0px";this.arrow2_node.style.left=(this.get_height()-this.get_width())+"px";this.arrow2_node.style.top="0px";}
}
;PureFW.Scrollbar.prototype.scroll_plus=function(){if(this._step_animation_enabled()){if(!this.scroll_ani_is_running){this._scroll_ani_plus_start();return;}
if(this.scroll_ani_steps_left<=0){this._scroll_ani_plus_stop();return;}
var hh=this.scroll_ani_step;this.scroll_ani_steps_left-=hh;}
else{var hh=this.scroll_step;}
this.document_pos++;if((this.document_size!==false)&&(this.document_size>=this.document_pos)){this.document_pos=this.document.size();this.stop_scroll();}
this.on_scroll(this.create_event("scroll",hh));}
;PureFW.Scrollbar.prototype.scroll_minus=function(){if(this._step_animation_enabled()){if(!this.scroll_ani_is_running){this._scroll_ani_minus_start();return;}
if(this.scroll_ani_steps_left<=0){this._scroll_ani_minus_stop();return;}
var hh=this.scroll_ani_step;this.scroll_ani_steps_left-=hh;}
else{var hh=this.scroll_step;}
this.document_pos--;if((this.document_size!==false)&&(this.document_size<=0)){this.document_pos=0;this.stop_scroll();}
this.on_scroll(this.create_event("scroll",hh));var jh=document.getElemById(this.content_id).style;}
;PureFW.Scrollbar.prototype.stop_scroll=function(){this._scroll_minus_stop();this._scroll_ani_minus_stop();this._scroll_plus_stop();this._scroll_ani_plus_stop();}
;PureFW.Scrollbar.prototype.add_event_handler=function(tc,hc){if(tc==="scroll")this.on_scroll_functions.push(hc);else
PureFW.Container.parent.add_event_handler.call(this,tc,hc);}
;PureFW.Scrollbar.prototype.remove_event_handler=function(tc,hc){if(tc==="change")this.on_change_functions.remove(hc);else
PureFW.Container.parent.remove_event_handler.call(this,tc,hc);}
;PureFW.Scrollbar.prototype.on_scroll=function(rb){if(!rb){rb=this.create_event("scroll");}
var n=this.on_scroll_functions.length;for(var i=0;i<n;i++){this.on_scroll_functions[i].call(this,rb);}
}
;PureFW.Scrollbar.prototype._scroll_plus_start=function(){this.scroll_plus();this.scroll_plus_interval_handler=window.setInterval("PureFW.Scrollbar.instances["+this.inst_num+"].scroll_plus()",this.scroll_interval
);}
;PureFW.Scrollbar.prototype._scroll_plus_stop=function(){try{window.clearInterval(this.scroll_plus_interval_handler);}
catch(e){}
}
;PureFW.Scrollbar.prototype._scroll_minus_start=function(){this.scroll_minus();this.scroll_minus_interval_handler=window.setInterval("PureFW.Scrollbar.instances["+this.inst_num+"].scroll_minus()",this.scroll_interval
);}
;PureFW.Scrollbar.prototype._scroll_minus_stop=function(){try{window.clearInterval(this.scroll_minus_interval_handler);}
catch(e){}
}
;PureFW.Scrollbar.prototype._scroll_ani_plus_start=function(){if(this._step_animation_enabled()){this.scroll_ani_steps_left=this.scroll_step;this.scroll_ani_is_running=true;this.scroll_ani_plus_interval_handler=window.setInterval("PureFW.Scrollbar.instances["+this.inst_num+"].scroll_plus()",this.scroll_ani_interval
);}
}
;PureFW.Scrollbar.prototype._scroll_ani_plus_stop=function(){this.scroll_ani_steps_left=0;this.scroll_ani_is_running=false;try{window.clearInterval(this.scroll_ani_plus_interval_handler);}
catch(e){}
}
;PureFW.Scrollbar.prototype._scroll_ani_minus_start=function(){if(this._step_animation_enabled()){this.scroll_ani_steps_left=this.scroll_step;this.scroll_ani_is_running=true;this.scroll_ani_minus_interval_handler=window.setInterval("PureFW.Scrollbar.instances["+this.inst_num+"].scroll_minus()",this.scroll_ani_interval
);}
}
;PureFW.Scrollbar.prototype._scroll_ani_minus_stop=function(){this.scroll_ani_steps_left=0;this.scroll_ani_is_running=false;try{window.clearInterval(this.scroll_ani_minus_interval_handler);}
catch(e){}
}
;PureFW.Scrollbar.prototype._step_animation_enabled=function(){return((this.scroll_ani_interval>0)&&((this.scroll_ani_step*this.scroll_ani_interval+100)<(this.scroll_interval*this.scroll_step)));}
PureFW.ScrollBox=function(xd,x,y,w,h,ad){this.node=null;this.container=null;this.content=null;this.scrollbar=null;if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h);this.vertical=ad?!!ad:false;this.scrollbar_width=15;this.instance_num=this.constructor.num_instances;this.id='Scrollbox'+this.instance_num;this.content_id='Scrollbox_cont'+this.instance_num;this.bg_img_id='Scrollbox_bg_img'+this.instance_num;this.insert_into_dom_tree();this.constructor.instances[this.instance_num]=this;this.constructor.num_instances++;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.ScrollBox.extend(PureFW.Container);PureFW.ScrollBox.num_instances=0;PureFW.ScrollBox.instances=new Array();PureFW.ScrollBox.prototype.insert_into_dom_tree=function(){PureFW.ScrollBox.parent.insert_into_dom_tree.call(this);var w=this.get_width();var h=this.get_height();if(this.vertical)w-=this.get_scrollbar_width();else
h-=this.get_scrollbar_width();this.container=new PureFW.Container(this.node,0,0,this.get_width(),this.get_height());var kh=this.container.get_node().style
kh.overflow='hidden';if(this.vertical){this.content=new PureFW.Container(this.container.get_node(),0,0,w,0);this.scrollbar=new PureFW.Scrollbar(this.container.get_node(),w,0,this.get_scrollbar_width(),this.get_height(),true);}
else{this.content=new PureFW.Container(this.container.get_node(),0,0,0,h);this.scrollbar=new PureFW.Scrollbar(this.container.get_node(),0,h,this.get_scrollbar_width(),this.get_width(),false);}
}
;PureFW.ScrollBox.prototype.get_scrollbar_width=function(){return this.scrollbar_width*this.scale_factor;}
;PureFW.ScrollBox.prototype.set_content=function(text,yb){this.on_change();this.content.set_content(text,yb);}
;PureFW.ScrollBox.prototype.add_content=function(text){this.on_change();this.content.add_content(text);}
;PureFW.ScrollBox.prototype.get_content=function(){return this.content.get_content();}
PureFW.ScrollBox.prototype.get_content_id=function(){return this.content.id;}
;PureFW.ScrollBox.prototype.set_scroll_step=function(hh){this.scrollbar.set_scroll_step(hh);}
;PureFW.ScrollBox.prototype.set_scroll_interval=function(ih){this.scrollbar.set_scroll_interval(ih);}
;PureFW.ScrollBox.prototype.set_animate_scroll=function(lh){this.scrollbar.set_animate_scroll(lh);}
;PureFW.ScrollBox.prototype.set_content_width=function(w){this.content.set_width(w);}
;PureFW.ScrollBox.prototype.on_scroll=function(rb){var jh=document.getElemById(this.content_id).style;var hh=rb.detail;if(this.vertical){var top=this.content.get_x();var mh=this.content.get_height()*(-1)+this.get_height();if((hh>0)&&(top>mh)){top-=hh;if(top<mh)top=mh;}
else if((hh<0)&&(top<0)){top-=hh;if(top>0)top=0;}
else{this.scroll_bar.stop_scroll();return;}
this.content.set_x(top);}
else{var ng=this.content.get_y();var nh=this.content.get_width()+this.get_width();if((hh>0)&&(ng>nh)){ng-=hh;if(ng<nh)ng=nh;}
else if((hh<0)&&(ng<0)){ng-=hh;if(ng>0)ng=0;}
else{this.scroll_bar.stop_scroll();return;}
this.content.set_y(ng);}
}
;PureFW.ScrollBox.prototype.destroy=function(){this.content.destroy();this.scrollbar.destroy();this.container.destroy();PureFW.ScrollBox.parent.destroy.call(this);}
;PureFW.TabBox=function(xd,x,y,w,h){if(typeof(xd)!=='undefined'){this.init();this.parent_node=xd||document.body;this.position=new PureFW.Point(x||0,y||0);this.width=w||100;this.height=h||100;this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.TabBox.instances["+this.instance_num+"]";this.id="TabBox"+this.instance_num;this.content_id="TabBox_cont"+this.instance_num;this.bg_img_id="TabBox_img"+this.instance_num;this.bg_img=null;this.active_tab=-1;this.tabs=new Array();this.constructor.instances[this.instance_num]=this;this.constructor.num_instances++;this.insert_into_dom_tree();PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.TabBox.extend(PureFW.Container);PureFW.TabBox.num_instances=0;PureFW.TabBox.instances=new Array();PureFW.TabBox.prototype.set_content=null;PureFW.TabBox.prototype.add_content=null;PureFW.TabBox.prototype.add_tab=function(oh,ph,qh,rh){var x,i;for(i=0;i<this.tabs.length;i++){this.tabs[i].deactivate();}
if(this.tabs.length<=0){x=oh;}
else{x=this.tabs[this.tabs.length-1].get_x()+this.tabs[this.tabs.length-1].get_width()+oh;}
var sh=this.tabs.length;this.tabs[sh]=new PureFW.TabBox_Tab(this.get_content_node(),x,ph,qh,rh,sh,this);this.tabs[sh].activate();this.active_tab=sh;return this.tabs[sh];}
;PureFW.TabBox.prototype.repaint=function(){PureFW.TabBox.parent.repaint.call(this);var n=this.tabs.length;for(var i=0;i<n;i++)this.tabs[i].repaint();}
;PureFW.TabBox.prototype.scale=function(s){PureFW.TabBox.parent.scale.call(this,s);var n=this.tabs.length;for(var i=0;i<n;i++)this.tabs[i].scale(s);}
;PureFW.TabBox.prototype.get_active_tab_index=function(){return this.active_tab;}
;PureFW.TabBox.prototype.get_tabs=function(){return this.tabs;}
;PureFW.TabBox.prototype.activate_tab=function(sh){var n=this.tabs.length;for(var i=0;i<n;i++){this.tabs[i].deactivate();}
this.tabs[sh].activate();this.active_tab=sh;}
;PureFW.TabBox.prototype.set_tab_content_padding_horz=function(hg){var n=this.tabs.length;for(var i=0;i<n;i++){this.tabs[i].set_content_padding_horz(hg);}
}
;PureFW.TabBox_Tab=function(xd,x,y,w,h,sh,th){if(typeof(xd)!=='undefined'){if(!xd){throw new Error('TabBox_Tab: parent_node have to be set in constructor');}
if(!th){throw new Error('TabBox_Tab: parent_tabbox have to be set in constructor');}
this.width=w||100;this.height=h||20;this.position=new PureFW.Point(x||0,y||0);this.parent_node=xd;this.parent_tabbox=th;this.css_class_suffix="";this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.TabBox_Tab.instances["+this.instance_num+"]";this.id='Tab_'+this.instance_num;this.init();this.bg_img_active=null;this.bg_img_inactive=null;this.tab_button=null;this.tab_content=null;this.internal_index=sh||0;this.active=false;this.padding_h=0;this.insert_into_dom_tree();this.constructor.instances[this.instance_num]=this;this.constructor.num_instances++;}
}
;PureFW.TabBox_Tab.extend(PureFW.Widget);PureFW.TabBox_Tab.num_instances=0;PureFW.TabBox_Tab.instances=new Array();PureFW.TabBox_Tab.tab_button_clicked=function(rb){var uh=PureFW.TabBox_Tab.instances[this.tab_instance_num];uh.parent_tabbox.activate_tab(uh.internal_index);}
;PureFW.TabBox_Tab.prototype.insert_into_dom_tree=function(){PureFW.TabBox_Tab.parent.insert_into_dom_tree.call(this);this.tab_button=new PureFW.Container(this.parent_node,this.get_x(),this.get_y(),this.get_width(),this.get_height());this.tab_button.tab_instance_num=this.instance_num;this.tab_button.add_event_handler("click",PureFW.TabBox_Tab.tab_button_clicked);this.tab_content=new PureFW.Container(this.parent_node,0,this.get_y()+this.get_height(),this.parent_tabbox.get_width(),this.parent_tabbox.get_height()-this.get_height());this.tab_content.hide();}
;PureFW.TabBox_Tab.prototype.repaint=function(){PureFW.TabBox_Tab.parent.repaint.call(this);}
;PureFW.TabBox_Tab.prototype.set_title=function(text){this.tab_button.set_content(text);}
;PureFW.TabBox_Tab.prototype.set_content=function(text){this.tab_content.set_content(text);}
;PureFW.TabBox_Tab.prototype.set_content_padding_horz=function(hg){this.padding_h=hg;this.tab_content.set_x(this.get_content_padding_horz());this.tab_content.set_width(this.parent_tabbox.get_width()-(this.get_content_padding_horz()<<1));}
;PureFW.TabBox_Tab.prototype.get_content_padding_horz=function(){return this.padding_h*this.scale_factor;}
;PureFW.TabBox_Tab.prototype.add_event_handler=function(tc,hc){if(tc.substr(0,7)==="button_"){this.tab_button.add_event_handler(tc.substr(7),hc);}
else{this.tab_content.add_event_handler(tc,hc);}
}
;PureFW.TabBox_Tab.prototype.remove_event_handler=function(tc,hc){if(tc.substr(0,7)==="button_"){this.tab_button.remove_event_handler(tc.substr(7),hc);}
else{this.tab_content.remove_event_handler(tc,hc);}
}
;PureFW.TabBox_Tab.prototype.get_content_id=function(){return this.tab_content.get_content_node().id;}
;PureFW.TabBox_Tab.prototype.get_button_width=function(){return this.get_width();}
;PureFW.TabBox_Tab.prototype.set_active_img=function(ub){this.bg_img_active=ub;if(this.active)this.activate();}
;PureFW.TabBox_Tab.prototype.set_inactive_img=function(ub){this.bg_img_inactive=ub;if(!this.active)this.deactivate();}
;PureFW.TabBox_Tab.prototype.activate=function(){this.tab_content.show();if(this.bg_img_active)this.tab_button.set_bg_img(this.bg_img_active);else
this.tab_button.set_bg_img('');this.active=true;}
;PureFW.TabBox_Tab.prototype.deactivate=function(){this.tab_content.hide();if(this.bg_img_inactive)this.tab_button.set_bg_img(this.bg_img_inactive);else
this.tab_button.set_bg_img('');this.active=true;}
;PureFW.ProgressBar=function(xd,x,y,w,h){if(typeof(xd)!=='undefined'){this.init();this.parent_node=xd||document;this.position=new PureFW.Point(x||0,y||0);this.width=w||142;this.height=h||15;this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.ProgressBar.instances["+this.instance_num+"]";this.id="ProgressBar"+this.instance_num;this.bg_img_id="ProgressBar_bg_img"+this.instance_num;this.content_id="ProgressBar_cont"+this.instance_num;this.node=null;this.empty_bar_container=null;this.background_url='ui/elements/progressbars/empty_142x15.png';this.progress_container=null;this.progress_value=null;this.progress_url='ui/elements/progressbars/full_142x15.png';this.timer_container=null;this.reference_value=100;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.ProgressBar.extend(PureFW.Container);PureFW.ProgressBar.num_instances=0;PureFW.ProgressBar.instances=new Array();PureFW.ProgressBar.prototype.insert_into_dom_tree=function(){PureFW.ProgressBar.parent.insert_into_dom_tree.call(this);this.empty_bar_container=new PureFW.Container(this.node,0,0,this.width,this.height);this.empty_bar_container.set_bg_img(this.background_url);this.empty_bar_container.set_z_index(2);this.progress_container=new PureFW.Container(this.node,0,0,0,this.height);this.progress_container.set_z_index(3);this.progress_container.get_node().style.overflow='hidden';this.progress_bar_container=new PureFW.Container(this.progress_container.get_content_node(),0,0,this.width,this.height);this.progress_bar_container.set_bg_img(this.progress_url);this.timer_container=new PureFW.Container(this.node,0,0,this.width,this.height);this.timer_container.get_node().style.textAlign='center';this.timer_container.set_font_weight('bold');this.timer_container.set_z_index(10);this.parent_node.appendChild(this.node);this.disable_selection();}
;PureFW.ProgressBar.prototype.set_value=function(v){this.progress_value=v;this._draw_current_progress();}
;PureFW.ProgressBar.prototype.get_value=function(){return this.progress_value;}
;PureFW.ProgressBar.prototype.set_reference_value=function(v){this.reference_value=v;this._draw_current_progress();}
;PureFW.ProgressBar.prototype.set_time=function(t){this.timer_container.set_content(t);this.timer_container.repaint();}
PureFW.ProgressBar.prototype._draw_current_progress=function(){var vh=this.progress_value/this.reference_value;this.progress_container.set_width(vh*this.width);this.repaint();}
;if(typeof(PureFW.IsoMap)!='object'){PureFW.IsoMap=new Object();}
PureFW.IsoMap.Map=new Object();for(x in PureFW.Widget)PureFW.IsoMap.Map[x]=PureFW.Widget[x];PureFW.IsoMap.Map.parent=PureFW.Widget;PureFW.IsoMap.Map.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.Map.parent.init.call(this,xd,x,y,w,h,yd);if(!wh||(wh<0))throw new Error("num_fields_x has to be in N1, "+wh+"given");if(!xh||(xh<0))throw new Error("num_fields_y has to be in N1, "+xh+"given");this.num_fields_x=wh;this.num_fields_y=xh;this.field_width=(this.width/this.num_fields_x);this.field_height=(this.height/this.num_fields_y);this.fields=new Array();}
;PureFW.IsoMap.Map.get_field_width=function(){return this.field_width*this.scale_factor;}
;PureFW.IsoMap.Map.get_field_height=function(){return this.field_height*this.scale_factor;}
;PureFW.IsoMap.Map.get_num_fields_x=function(){return this.num_fields_x;}
PureFW.IsoMap.Map.get_num_fields_y=function(){return this.num_fields_y;}
PureFW.IsoMap.Map.get_field_pixel_position=function(x,y){ib=new PureFW.Point(this.width+this.field_width*(x-y-1)>>1,(this.field_height>>1)*(x+y));return ib;}
;PureFW.IsoMap.PseudoClickMap=function(xd,x,y,w,h,wh,xh,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,wh,xh,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.IsoMap.PseudoClickMap.instances["+this.instance_num+"]";this.id="PureFW.IsoMap.PseudoClickMap"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.IsoMap.PseudoClickMap.extend(PureFW.IsoMap.Map);PureFW.IsoMap.PseudoClickMap.num_instances=0;PureFW.IsoMap.PseudoClickMap.instances=new Array();PureFW.IsoMap.PseudoClickMap.Field=function(){this.enabled=true;}
PureFW.IsoMap.PseudoClickMap.Field.prototype.destroy=function(){}
PureFW.IsoMap.PseudoClickMap.prototype.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.PseudoClickMap.parent.init.call(this,xd,x,y,w,h,wh,xh,yd);for(var x=0;x<this.num_fields_x;x++){this.fields[x]=new Array();for(var y=0;y<this.num_fields_y;y++){this.fields[x][y]=null;}
}
this.field_widgets=new Array();this.field_widgets_z_index=0;this.on_field_click_functions=new Array();this.on_field_mouseover_functions=new Array();this.on_field_mouseout_functions=new Array();this.need_area_recreate=false;}
;PureFW.IsoMap.PseudoClickMap.prototype.insert_into_dom_tree=function(){var yh='<div id="'+this.id+'"'+' style="position: absolute; top: '+this.position.y+'px;'+' left: '+(this.position.x)+'px;'+' width:'+(this.width)+'px; height:'+(this.height)+'px; '+'z-index: '+(this.z_index)+';"></div>';this.parent_node.innerHTML=this.parent_node.innerHTML+yh;}
;PureFW.IsoMap.PseudoClickMap.prototype.on_field_click=function(rb,i,j){if(!this.fields[i][j].enabled)return;if(n==0)return;var n=this.on_field_click_functions.length;zh=null;if(rb){try{rb=PureFW.EventUtil.formatEvent(rb);rb.detail=new PureFW.Point(i,j);if(typeof(rb.detail)!='object')throw"Event is not changable";}
catch(e){zh=this.create_event("click");zh.detail=new PureFW.Point(i,j);}
}
else{rb=this.create_event("click");rb.detail=new PureFW.Point(i,j);}
for(var i=0;i<n;i++){this.on_field_click_functions[i].call(this,zh||rb);}
}
PureFW.IsoMap.PseudoClickMap.prototype.on_field_mouseover=function(rb,i,j){if(!this.fields[i][j].enabled)return;var n=this.on_field_mouseover_functions.length;if(n==0)return;zh=null;if(rb){try{rb=PureFW.EventUtil.formatEvent(rb);rb.detail=new PureFW.Point(i,j);if(typeof(rb.detail)!='object')throw"Event is not changable";}
catch(e){zh=this.create_event("mouseover");zh.detail=new PureFW.Point(i,j);}
}
else{rb=this.create_event("mouseover");rb.detail=new PureFW.Point(i,j);}
for(var i=0;i<n;i++)this.on_field_mouseover_functions[i].call(this,zh||rb);}
PureFW.IsoMap.PseudoClickMap.prototype.on_field_mouseout=function(rb,i,j){if(!this.fields[i][j].enabled)return;var n=this.on_field_mouseout_functions.length;if(n==0)return;zh=null;if(rb){try{rb=PureFW.EventUtil.formatEvent(rb);rb.detail=new PureFW.Point(i,j);if(typeof(rb.detail)!='object')throw"Event is not changable";}
catch(e){zh=this.create_event("mouseout");zh.detail=new PureFW.Point(i,j);}
}
else{rb=this.create_event("mouseout");rb.detail=new PureFW.Point(i,j);}
for(var i=0;i<n;i++)this.on_field_mouseout_functions[i].call(this,zh||rb);}
PureFW.IsoMap.PseudoClickMap.prototype.add_event_handler=function(tc,hc){if(tc=="field_mouseover"){this.on_field_mouseover_functions.push(hc);}
else if(tc=="field_mouseout"){this.on_field_mouseout_functions.push(hc);}
else if(tc=="field_click"){this.on_field_click_functions.push(hc);}
else{PureFW.IsoMap.PseudoClickMap.parent.add_event_handler.call(this,tc,hc);}
}
PureFW.IsoMap.PseudoClickMap.prototype.remove_event_handler=function(tc,hc){if(tc=="field_mouseover"){this.on_field_mouseover_functions.remove(hc);}
else if(tc=="field_mouseout"){this.on_field_mouseout_functions.remove(hc);}
else if(tc=="field_click"){this.on_field_click_functions.remove(hc);}
else{PureFW.IsoMap.PseudoClickMap.parent.remove_event_handler.call(this,tc,hc);}
}
PureFW.IsoMap.PseudoClickMap.prototype.destroy=function(){PureFW.IsoMap.PseudoClickMap.parent.destroy.call(this);this.field_widgets.destroy();this.fields.destroy();}
;PureFW.IsoMap.PseudoClickMap.prototype.create_field=function(x,y){if(!this.fields[x][y]){this.fields[x][y]=new PureFW.IsoMap.PseudoClickMap.Field();this.need_area_recreate=true;}
}
PureFW.IsoMap.PseudoClickMap.prototype.destroy_field=function(x,y){if(this.fields[x][y]){try{this.fields[x][y].destroy();}
catch(e){}
this.fields[x][y]=null;this.need_area_recreate=true;}
}
PureFW.IsoMap.PseudoClickMap.prototype.refresh=function(){if(this.need_area_recreate){this.field_widgets.destroy();for(var i=0;i<this.num_fields_x;i++){for(var j=0;j<this.num_fields_y;j++){if(!this.fields[i][j])continue;var x=(this.width>>1)+(((i-j-1)*this.field_width+(this.field_width>>1))>>1);var y=(((i+j)*this.field_height+(this.field_height>>1))>>1);var af=new PureFW.Image(this,x,y,this.field_width>>1,this.field_height>>1,'pattern/spacer.gif',this.no_scale
);af.instance_name="MammunUI.get_top_frame()."+af.instance_name;af.add_event_handler("click",(function(be,$h,_h){return function(rb){be.on_field_click(rb,$h,_h)}
}
)(this,i,j));af.add_event_handler("mouseover",(function(be,$h,_h){return function(rb){be.on_field_mouseover(rb,$h,_h)}
}
)(this,i,j));af.add_event_handler("mouseout",(function(be,$h,_h){return function(rb){be.on_field_mouseout(rb,$h,_h)}
}
)(this,i,j));af.set_z_index(this.field_widgets_z_index);this.field_widgets.push(af);}
}
}
}
;PureFW.IsoMap.PseudoClickMap.prototype.activate_field=function(x,y){if(x<0||x>=this.num_fields_x)throw new Error("x has to be in {0,1, .., "+this.num_fields_x+". '"+x+"' given.");if(y<0||y>=this.num_fields_y)throw new Error("y has to be in {0,1, .., "+this.num_fields_y+". '"+y+"' given.");this.fields[x][y].enabled=true;}
;PureFW.IsoMap.PseudoClickMap.prototype.deactivate_field=function(x,y){if(x<0||x>=this.num_fields_x)throw new Error("x has to be in {0,1, .., "+this.num_fields_x+". '"+x+"' given.");if(y<0||y>=this.num_fields_y)throw new Error("y has to be in {0,1, .., "+this.num_fields_y+". '"+y+"' given.");this.fields[x][y].enabled=false;}
;PureFW.IsoMap.ClickMap=function(xd,x,y,w,h,wh,xh,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,wh,xh,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.IsoMap.ClickMap.instances["+this.instance_num+"]";this.id="PureFW.IsoMap.ClickMap"+this.instance_num;this.img_id="PureFW.IsoMap.ClickMap_img"+this.instance_num;this.map_id='PureFW.IsoMap.ClickMap_map'+this.instance_num;this.area_id_prefix="PureFW.IsoMap.ClickMap_area"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.IsoMap.ClickMap.extend(PureFW.IsoMap.PseudoClickMap);PureFW.IsoMap.ClickMap.num_instances=0;PureFW.IsoMap.ClickMap.instances=new Array();PureFW.IsoMap.ClickMap.Field=function(){this.enabled=true;}
PureFW.IsoMap.ClickMap.Field.prototype.destroy=function(){}
PureFW.IsoMap.ClickMap.prototype.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.ClickMap.parent.init.call(this,xd,x,y,w,h,wh,xh,yd);for(var x=0;x<this.num_fields_x;x++){this.fields[x]=new Array();for(var y=0;y<this.num_fields_y;y++){this.fields[x][y]=null;}
}
this.on_field_click_functions=new Array();this.on_field_mouseover_functions=new Array();this.on_field_mouseout_functions=new Array();this.need_area_recreate=false;}
;PureFW.IsoMap.ClickMap.prototype.insert_into_dom_tree=function(){var yh='<div id="'+this.id+'"'+' style="position: absolute; top: '+this.position.y+'px;'+' left: '+(this.position.x)+'px;'+' width:'+(this.width)+'px; height:'+(this.height)+'px; '+'z-index: '+(this.z_index)+';">';yh+='<img id="'+this.img_id+'" border="0" width="100%" height="100%"'+' src="../pix/pattern/spacer.gif" usemap="#'+this.map_id+'"'+' style="position: absolute; top: 0px; left: 0px; z-index: 0"/>';yh+='<map id="'+this.map_id+'" name="'+this.map_id+'">'+this._generate_map_inner_html()+'</map>';yh+='</div>';this.parent_node.innerHTML=this.parent_node.innerHTML+yh;}
;PureFW.IsoMap.ClickMap.prototype._generate_map_inner_html=function(){var ai=this.field_width;var bi=this.field_height;var ci=ai*this.scale_factor;var di=bi*this.scale_factor;var ei=di>>1;var fi=ci>>1;var x,y;var gi='';for(var j=0;j<this.num_fields_y;j++){for(var i=0;i<this.num_fields_x;i++){if(this.fields[i][j]){var x=((this.width>>1)+(((i-j-1)*ai)>>1))*this.scale_factor;var y=(((i+j)*bi)>>1)*this.scale_factor;gi+='<area id="'+this.area_id_prefix+'_'+i+'_'+j+'"';gi+=' href="javascript: ;"';gi+=' onclick="javascript: '+this.instance_name+'.on_field_click(event, '+i+','+j+');"';gi+=' onmouseover="javascript: '+this.instance_name+'.on_field_mouseover(event, '+i+','+j+');"';gi+=' onmouseout="javascript: '+this.instance_name+'.on_field_mouseout(event, '+i+','+j+');"';gi+=' shape="poly" coords="'+(x)+','+(y+ei)+', '+(x+fi)+','+(y)+', '+(x+ci)+','+(y+ei)+', '+(x+fi)+','+(y+di)+'"/>';}
}
}
return gi;}
PureFW.IsoMap.ClickMap.prototype.destroy=function(){PureFW.IsoMap.ClickMap.parent.destroy.call(this);this.fields.destroy();}
;PureFW.IsoMap.ClickMap.prototype.refresh=function(){if(this.need_area_recreate){document.setElemInnerHTML(this.map_id,this._generate_map_inner_html());this.need_area_recreate=false;}
}
PureFW.IsoMap.ClickMap3D=function(xd,x,y,w,h,wh,xh,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,wh,xh,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.IsoMap.ClickMap3D.instances["+this.instance_num+"]";this.id="PureFW.IsoMap.ClickMap3D"+this.instance_num;this.img_id="PureFW.IsoMap.ClickMap3D_img"+this.instance_num;this.map_id='PureFW.IsoMap.ClickMap3D_map'+this.instance_num;this.area_id_prefix="PureFW.IsoMap.ClickMap3D_area"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.IsoMap.ClickMap3D.extend(PureFW.IsoMap.ClickMap);PureFW.IsoMap.ClickMap3D.num_instances=0;PureFW.IsoMap.ClickMap3D.instances=new Array();PureFW.IsoMap.ClickMap3D.MapObject=function(x,y,w,h,hi,ii){this.enabled=true;this.size=new PureFW.Point(w,h);this.pos=new PureFW.Point(x,y);this.proj_h=hi;this.tooltip=ii;}
PureFW.IsoMap.ClickMap3D.MapObject.prototype.destroy=function(){delete this.size;this.size=null;}
PureFW.IsoMap.ClickMap3D.prototype.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.ClickMap3D.parent.init.call(this,xd,x,y,w,h,wh,xh,yd);this.objects_3d=new Array();for(var x=0;x<this.num_fields_x;x++){this.objects_3d[x]=new Array();for(var y=0;y<this.num_fields_y;y++){this.objects_3d[x][y]=null;}
}
this.on_object_click_functions=new Array();this.on_object_mouseover_functions=new Array();this.on_object_mouseout_functions=new Array();this.img_y=0;}
;PureFW.IsoMap.ClickMap3D.prototype.insert_into_dom_tree=function(){PureFW.IsoMap.ClickMap3D.parent.insert_into_dom_tree.call(this);}
;PureFW.IsoMap.ClickMap3D.prototype._generate_map_inner_html=function(){var x,y;var gi='';var yc=Infinity;for(var j=0;j<this.num_fields_y;j++){for(var i=0;i<this.num_fields_x;i++){if(this.objects_3d[i][j]){var y=((this.field_height*(i+j)>>1)-this.objects_3d[i][j].size.y*this.field_height
*(this.objects_3d[i][j].proj_h-1))*this.scale_factor;var ji=this.objects_3d[i][j].size.y*this.field_height
*this.scale_factor;var ki=this.objects_3d[i][j].size.y*this.objects_3d[i][j].proj_h*this.field_height
*this.scale_factor;this.objects_3d[i][j].tmp_y=y;this.objects_3d[i][j].tmp_bbaseh=ji;this.objects_3d[i][j].tmp_bh=ki;y-=ki-(ji>>1);if(y<yc)yc=y;}
}
}
for(var j=0;j<this.num_fields_y;j++){for(var i=0;i<this.num_fields_x;i++){if(this.objects_3d[i][j]){var li=this.objects_3d[i][j].size.x*this.field_width;var ki=this.objects_3d[i][j].tmp_bh;var ji=this.objects_3d[i][j].tmp_bbaseh;var x=((this.width+this.field_width
*(i-j-this.objects_3d[i][j].size.x))>>1)*this.scale_factor;var y=this.objects_3d[i][j].tmp_y-yc;gi='<area id="'+this.area_id_prefix+'_'+i+'_'+j+'"'+' href="javascript: ;"'+' onclick="javascript: '+this.instance_name+'.on_object_click(event, '+i+','+j+');"'+' onmouseover="javascript: '+this.instance_name+'.on_object_mouseover(event, '+i+','+j+');"'+' onmouseout="javascript: '+this.instance_name+'.on_object_mouseout(event, '+i+','+j+');"'+' shape="poly" coords="'+x+', '+(y+(ji>>1))+', '+(x+(li>>1))+', '+y+', '+(x+li)+', '+(y+(ji>>1))+', '+(x+li)+', '+(y+(ki-(ji>>1)))+', '+(x+(li>>1))+', '+(y+ki)+', '+x+', '+(y+(ki-(ji>>1)))+'"/>'+gi;}
}
}
this.img_y=yc;return gi;}
PureFW.IsoMap.ClickMap3D.prototype.repaint=function(){PureFW.IsoMap.ClickMap3D.parent.repaint.call(this);this.refresh();}
PureFW.IsoMap.ClickMap3D.prototype.on_object_click=function(rb,i,j){if(!this.objects_3d[i][j].enabled)return;var n=this.on_object_click_functions.length;if(n==0)return;if(rb){try{rb=PureFW.EventUtil.formatEvent(rb);rb.detail=this.objects_3d[i][j];if(typeof(rb.detail)!='object')throw"Event is not changable";}
catch(e){rb=this.create_event("click");rb.detail=this.objects_3d[i][j];}
}
else{rb=this.create_event("click");rb.detail=this.objects_3d[i][j];}
for(var i=0;i<n;i++){this.on_object_click_functions[i].call(this,rb);}
}
PureFW.IsoMap.ClickMap3D.prototype.on_object_mouseover=function(rb,i,j){if(!this.objects_3d[i][j].enabled)return;var n=this.on_object_mouseover_functions.length;if(n==0)return;if(rb){try{rb=PureFW.EventUtil.formatEvent(rb);rb.detail=this.objects_3d[i][j];if(typeof(rb.detail)!='object')throw"Event is not changable";}
catch(e){rb=this.create_event("mouseover");rb.detail=this.objects_3d[i][j];}
}
else{rb=this.create_event("mouseover");rb.detail=this.objects_3d[i][j];}
for(var i=0;i<n;i++)this.on_object_mouseover_functions[i].call(this,rb);}
PureFW.IsoMap.ClickMap3D.prototype.on_object_mouseout=function(rb,i,j){if(!this.objects_3d[i][j].enabled)return;var n=this.on_object_mouseout_functions.length;if(n==0)return;if(rb){try{rb=PureFW.EventUtil.formatEvent(rb);rb.detail=this.objects_3d[i][j];if(typeof(rb.detail)!='object')throw"Event is not changable";}
catch(e){rb=this.create_event("mouseout");rb.detail=this.objects_3d[i][j];}
}
else{rb=this.create_event("mouseout");rb.detail=this.objects_3d[i][j];}
for(var i=0;i<n;i++)this.on_object_mouseout_functions[i].call(this,rb);}
PureFW.IsoMap.ClickMap3D.prototype.add_event_handler=function(tc,hc){if(tc=="object_mouseover"){this.on_object_mouseover_functions.push(hc);}
else if(tc=="object_mouseout"){this.on_object_mouseout_functions.push(hc);}
else if(tc=="object_click"){this.on_object_click_functions.push(hc);}
else{PureFW.IsoMap.ClickMap3D.parent.add_event_handler.call(this,tc,hc);}
}
PureFW.IsoMap.ClickMap3D.prototype.remove_event_handler=function(tc,hc){if(tc=="object_mouseover"){this.on_object_mouseover_functions.remove(hc);}
else if(tc=="object_mouseout"){this.on_object_mouseout_functions.remove(hc);}
else if(tc=="object_click"){this.on_object_click_functions.remove(hc);}
else{PureFW.IsoMap.ClickMap3D.parent.remove_event_handler.call(this,tc,hc);}
}
PureFW.IsoMap.ClickMap3D.prototype.destroy=function(){PureFW.IsoMap.ClickMap3D.parent.destroy.call(this);this.objects_3d.destroy();}
;PureFW.IsoMap.ClickMap3D.prototype.create_object=function(x,y,mi,ni,oi,ii){if(!this.objects_3d[x][y]){this.objects_3d[x][y]=new PureFW.IsoMap.ClickMap3D.MapObject(x,y,mi,ni,oi,ii
);this.need_area_recreate=true;}
return this.objects_3d[x][y];}
PureFW.IsoMap.ClickMap3D.prototype.destroy_object=function(x,y){if(this.objects_3d[x][y]){try{this.objects_3d[x][y].destroy();}
catch(e){}
this.objects_3d[x][y]=null;this.need_area_recreate=true;}
}
PureFW.IsoMap.ClickMap3D.prototype.get_object=function(x,y){return(this.objects_3d[x][y])?this.objects_3d[x][y]:null;}
PureFW.IsoMap.ClickMap3D.prototype.refresh=function(){if(this.need_area_recreate){PureFW.IsoMap.ClickMap3D.parent.refresh.call(this);document.getElemById(this.img_id).height=this.height-this.img_y;document.getElemById(this.img_id).style.top=this.img_y+"px";}
}
PureFW.IsoMap.ClickMap3D.prototype.activate_object=function(x,y){if(x<0||x>=this.num_fields_x)throw new Error("x has to be in {0,1, .., "+this.num_fields_x+". '"+x+"' given.");if(y<0||y>=this.num_fields_y)throw new Error("y has to be in {0,1, .., "+this.num_fields_y+". '"+y+"' given.");this.objects_3d[x][y].enabled=true;}
;PureFW.IsoMap.ClickMap3D.prototype.deactivate_object=function(x,y){if(x<0||x>=this.num_fields_x)throw new Error("x has to be in {0,1, .., "+this.num_fields_x+". '"+x+"' given.");if(y<0||y>=this.num_fields_y)throw new Error("y has to be in {0,1, .., "+this.num_fields_y+". '"+y+"' given.");this.objects_3d[x][y].enabled=false;}
;PureFW.IsoMap.VisMap=function(xd,x,y,w,h,wh,xh,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,wh,xh,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.IsoMap.VisMap.instances["+this.instance_num+"]";this.id="PureFW.IsoMap.VisMap"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
PureFW.IsoMap.VisMap.extend(PureFW.IsoMap.Map);PureFW.IsoMap.VisMap.num_instances=0;PureFW.IsoMap.VisMap.instances=new Array();PureFW.IsoMap.VisMap.prototype.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.VisMap.parent.init.call(this,xd,x,y,w,h,wh,xh,yd);for(var x=0;x<this.num_fields_x;x++){this.fields[x]=new Array();for(var y=0;y<this.num_fields_y;y++){this.fields[x][y]=null;}
}
}
;PureFW.IsoMap.VisMap.prototype.insert_into_dom_tree=function(){PureFW.IsoMap.VisMap.parent.insert_into_dom_tree.call(this);this.node=document.createElement('div');this.node.style.position='absolute';this.node.style.width=this.get_width()+'px';this.node.style.height=this.get_height()+'px';this.node.style.left=this.get_x()+'px';this.node.style.top=this.get_y()+'px';this.node.style.overflow='visible';this.node.id=this.id;this.parent_node.appendChild(this.node);}
PureFW.IsoMap.VisMap.prototype.create_field=function(x,y,jf,pi,qi,ri,si,ti,ui,vi,wi){this.destroy_field(x,y);var xi=(this.width+this.field_width*(x-y-1))>>1;var yi=this.field_height*(x+y)>>1;this.fields[x][y]=new jf(this,xi,yi,this.field_width,this.field_height,pi,qi,ri,si,ti,ui,vi,wi
);this.fields[x][y].set_z_index(x+y);return this.fields[x][y];}
PureFW.IsoMap.VisMap.prototype.get_field=function(x,y){return this.fields[x][y];}
PureFW.IsoMap.VisMap.prototype.destroy_field=function(x,y){if(this.fields[x][y]){try{this.fields[x][y].destroy();}
catch(e){}
this.fields[x][y]=null;}
}
PureFW.IsoMap.VisMap3D=function(xd,x,y,w,h,wh,xh,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,wh,xh,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.IsoMap.VisMap3D.instances["+this.instance_num+"]";this.id="PureFW.IsoMap.VisMap3D"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
PureFW.IsoMap.VisMap3D.extend(PureFW.IsoMap.VisMap);PureFW.IsoMap.VisMap3D.num_instances=0;PureFW.IsoMap.VisMap3D.instances=new Array();PureFW.IsoMap.VisMap3D.MapObject=function(dd,w,h,oi){this.widget=dd;this.info=new Object();this.info.size=new PureFW.Point(w,h);this.info.proj_h=oi;}
PureFW.IsoMap.VisMap3D.MapObject.prototype.destroy=function(){try{this.widget.destroy();}
catch(e){}
try{this.info.size.destroy();}
catch(e){}
delete this.info;}
PureFW.IsoMap.VisMap3D.prototype.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.VisMap3D.parent.init.call(this,xd,x,y,w,h,wh,xh,yd);this.objects_3d=new Array();for(var i=0;i<this.num_fields_x;i++){this.objects_3d[i]=new Array();for(var j=0;j<this.num_fields_y;j++)this.objects_3d[i][j]=null;}
}
;PureFW.IsoMap.VisMap3D.prototype.insert_into_dom_tree=function(){PureFW.IsoMap.VisMap3D.parent.insert_into_dom_tree.call(this);}
PureFW.IsoMap.VisMap3D.prototype.create_object=function(x,y,mi,ni,oi,jf,pi,qi,ri,si,ti,ui,vi,wi){this.destroy_object(x,y);var xi=(this.width+this.field_width*(x-y-ni))>>1;var yi=(this.field_height*(x+y)>>1)-ni*this.field_height*(oi-1);var zi=this.field_width*mi;var $i=this.field_height*ni*oi;dd=new jf(this,xi,yi,zi,$i,pi,qi,ri,si,ti,ui,vi,wi
);dd.set_z_index(x+y+this.num_fields_x+this.num_fields_y);this.objects_3d[x][y]=new PureFW.IsoMap.VisMap3D.MapObject(dd,mi,ni,oi
);return dd;}
;PureFW.IsoMap.VisMap3D.prototype.destroy_object=function(x,y){if(this.objects_3d[x][y]){try{this.objects_3d[x][y].destroy();}
catch(e){}
this.objects_3d[x][y]=null;}
}
PureFW.IsoMap.VisMap3D.prototype.get_object=function(x,y,_i){if(_i)return(this.objects_3d[x][y])?this.objects_3d[x][y]:null;else
return(this.objects_3d[x][y])?this.objects_3d[x][y].widget:null;}
;PureFW.IsoMap.PseudoMultilayerMap=function(xd,x,y,w,h,wh,xh,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,wh,xh,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.IsoMap.PseudoMultilayerMap.instances["+this.instance_num+"]";this.id="PureFW.IsoMap.PseudoMultilayerMap"+this.instance_num;this.img_id="PureFW.IsoMap.PseudoMultilayerMap_img"+this.instance_num;this.map_id='PureFW.IsoMap.PseudoMultilayerMap_map'+this.instance_num;this.area_id_prefix="PureFW.IsoMap.PseudoMultilayerMap_area"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.IsoMap.PseudoMultilayerMap.extend(PureFW.IsoMap.PseudoClickMap);PureFW.IsoMap.PseudoMultilayerMap.num_instances=0;PureFW.IsoMap.PseudoMultilayerMap.instances=new Array();PureFW.IsoMap.PseudoMultilayerMap.prototype.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.PseudoMultilayerMap.parent.init.call(this,xd,x,y,w,h,wh,xh,yd);this.vis_layers=new Object();}
;PureFW.IsoMap.PseudoMultilayerMap.prototype.insert_into_dom_tree=function(){PureFW.IsoMap.PseudoMultilayerMap.parent.insert_into_dom_tree.call(this);}
;PureFW.IsoMap.PseudoMultilayerMap.prototype.add_layer=function(aj,bj){this.vis_layers[aj]=new PureFW.IsoMap.VisMap(this,0,0,this.width,this.height,this.num_fields_x,this.num_fields_y,this.no_scale
);if(typeof(bj)!='undefined'){this.vis_layers[aj].set_z_index(bj);if(this.field_widgets_z_index<=bj){var n=this.field_widgets.length;this.field_widgets_z_index=bj+1;for(var i=0;i<n;i++)this.field_widgets[i].set_z_index(bj+1);}
}
return this.vis_layers[aj];}
PureFW.IsoMap.PseudoMultilayerMap.prototype.remove_layer=function(aj){if(typeof(this.vis_layers[aj])!='undefined'){try{this.vis_layers[aj].destroy();}
catch(e){}
this.vis_layers[aj]=null;}
}
PureFW.IsoMap.PseudoMultilayerMap.prototype.get_layer=function(aj){return this.vis_layers[aj];}
PureFW.IsoMap.MultilayerMap=function(xd,x,y,w,h,wh,xh,yd){if(typeof(xd)!=='undefined'){this.init(xd,x,y,w,h,wh,xh,yd);this.instance_num=this.constructor.num_instances;this.instance_name="PureFW.IsoMap.MultilayerMap.instances["+this.instance_num+"]";this.id="PureFW.IsoMap.MultilayerMap"+this.instance_num;this.img_id="PureFW.IsoMap.MultilayerMap_img"+this.instance_num;this.map_id='PureFW.IsoMap.MultilayerMap_map'+this.instance_num;this.area_id_prefix="PureFW.IsoMap.MultilayerMap_area"+this.instance_num;this.insert_into_dom_tree();this.constructor.num_instances++;this.constructor.instances[this.instance_num]=this;if(!this.no_scale)PureFW.WidgetManager.manager_all.register_widget(this);}
}
;PureFW.IsoMap.MultilayerMap.extend(PureFW.IsoMap.ClickMap);PureFW.IsoMap.MultilayerMap.num_instances=0;PureFW.IsoMap.MultilayerMap.instances=new Array();PureFW.IsoMap.MultilayerMap.prototype.init=function(xd,x,y,w,h,wh,xh,yd){PureFW.IsoMap.MultilayerMap.parent.init.call(this,xd,x,y,w,h,wh,xh,yd);this.vis_layers=new Object();}
;PureFW.IsoMap.MultilayerMap.prototype.insert_into_dom_tree=function(){PureFW.IsoMap.MultilayerMap.parent.insert_into_dom_tree.call(this);}
;PureFW.IsoMap.MultilayerMap.prototype.add_layer=function(aj,bj){this.vis_layers[aj]=new PureFW.IsoMap.VisMap(this,0,0,this.width,this.height,this.num_fields_x,this.num_fields_y,this.no_scale
);if(typeof(bj)!='undefined'){this.vis_layers[aj].set_z_index(bj);cj=document.getElemById(this.img_id);if(bj>=cj.style.zIndex)cj.style.zIndex=bj+1;}
return this.vis_layers[aj];}
PureFW.IsoMap.MultilayerMap.prototype.remove_layer=function(aj){if(typeof(this.vis_layers[aj])!='undefined'){try{this.vis_layers[aj].destroy();}
catch(e){}
this.vis_layers[aj]=null;}
}
PureFW.IsoMap.MultilayerMap.prototype.get_layer=function(aj){return this.vis_layers[aj];}
