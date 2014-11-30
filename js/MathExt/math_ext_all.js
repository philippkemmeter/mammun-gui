MathExt=new Object();MathExt.Random=new Object();MathExt.Random.halton=function(ab,bb){var cb=0;var f=1/bb;var i=ab;while(i>0){cb+=f*(i%bb);i=parseInt((i)/bb);f=f/bb;}
return cb;}
;if(typeof(MathExt.LinAlg2D)=='undefined')MathExt.LinAlg2D=new Object();MathExt.LinAlg2D.Vector=function(x,y){this.x=Number(x);this.y=Number(y);}
;MathExt.LinAlg2D.Vector.prototype.add=function(p){return new MathExt.LinAlg2D.Vector(this.x+p.x,this.y+p.y);}
;MathExt.LinAlg2D.Vector.prototype.substract=function(p){return new MathExt.LinAlg2D.Vector(this.x-p.x,this.y-p.y);}
;MathExt.LinAlg2D.Vector.prototype.dot_product=function(p){return this.x*p.x+this.y*p.y;}
;MathExt.LinAlg2D.Vector.prototype.scalar_mult=function(s){return new MathExt.LinAlg2D.Vector(this.x*s,this.y*s);}
;MathExt.LinAlg2D.Vector.prototype.get_vector_length=function(){return Math.sqrt(this.dot_product(this));}
;MathExt.LinAlg2D.Line=function(m,b){this.m=Number(m);this.b=Number(b);}
;MathExt.LinAlg2D.Line.prototype.get_direction_vector=function(){return new MathExt.LinAlg2D.Vector(this.m,1);}
;MathExt.LinAlg2D.calc_line_intersection=function(db,eb){if(db.m==eb.m)return new MathExt.LinAlg2D.Vector(Infinity,Infinity);return new MathExt.LinAlg2D.Vector((eb.b-db.b)/(db.m-eb.m),(db.m*eb.b-eb.m*db.b)/(db.m-eb.m));}
;MathExt.LinAlg2D.calc_line_mb_by_points=function(p,q){if(q.x==p.x)return new MathExt.LinAlg2D.Line(Infinity,-Infinity);if(q.y==p.y)return new MathExt.LinAlg2D.Line(0,p.y);cb=new MathExt.LinAlg2D.Line();cb.m=(q.y-p.y)/(q.x-p.x);cb.b=(p.y-cb.m*p.x);return cb;}
;