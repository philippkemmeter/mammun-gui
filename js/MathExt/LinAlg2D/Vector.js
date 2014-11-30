if (typeof(MathExt.LinAlg2D) == 'undefined')
	MathExt.LinAlg2D = new Object();

/**
 * Ein einfacher 2D-Vektor mit Operationen.
 * 
 * @param float x
 * @param float y
 */
MathExt.LinAlg2D.Vector = function(x, y) {
	this.x = Number(x);
	this.y = Number(y);
};

/**
 * Addiert den übergebenen Vektor zum aktuellen und gibt einen neuen Vektor
 * zurück, der das Ergebnis enthält.
 * 
 * @param MathExt.LinAlg2D.Vector p
 * @return MathExt.LinAlg2D.Vector
 */
MathExt.LinAlg2D.Vector.prototype.add = function(p) {
	return new MathExt.LinAlg2D.Vector(this.x+p.x, this.y+p.y);
};

MathExt.LinAlg2D.Vector.prototype.substract = function(p) {
	return new MathExt.LinAlg2D.Vector(this.x-p.x, this.y-p.y);
};

/**
 * Berechnet das Skalarprodukt des Vektores mit dem übergebenen Vektor p.
 * 
 * @param MathExt.LinAlg2D.Vector p
 * @return float
 */
MathExt.LinAlg2D.Vector.prototype.dot_product = function(p) {
	return this.x*p.x + this.y*p.y;
};

/**
 * Berechnet die Skalarmultiplikation des Vektores.
 * 
 * @param float s
 * @return MathExt.LinAlg2D.Vector
 */
MathExt.LinAlg2D.Vector.prototype.scalar_mult = function(s) {
	return new MathExt.LinAlg2D.Vector(this.x*s, this.y*s);
};

MathExt.LinAlg2D.Vector.prototype.get_vector_length = function() {
	return Math.sqrt(this.dot_product(this));
};