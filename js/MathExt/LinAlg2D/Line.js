/**
 * Struktur zur Geradendefinition durch Steigung und y-Achsenabschnitt.
 * 
 * @param float m	Steigung
 * @param float b	y-Achsenabschnitt
 */
MathExt.LinAlg2D.Line = function (m, b) {
	this.m = Number(m);
	this.b = Number(b);
};

/**
 * Gibt den Richtungsvektor der Gerade zurück.
 * 
 * @return MathExt.LinAlg2D.Vector
 */
MathExt.LinAlg2D.Line.prototype.get_direction_vector = function() {
	return new MathExt.LinAlg2D.Vector(this.m, 1);
};

/**
 * Berechnet den Schnittpunkt zweier Gerade im 2D-Raum, gegeben durch ihre
 * Steigung und y-Achsenabschnitt (Math.LinAlg2D.LineMB).
 * 
 * @param MathExt.LinAlg2D.Line l1	Die erste Gerade
 * @param MathExt.LinAlg2D.Line l2	Die zweite Gerade
 * @result MathExt.LinAlg2D.Vector
 */
MathExt.LinAlg2D.calc_line_intersection = function(l1, l2) {
	if (l1.m == l2.m)
		return new MathExt.LinAlg2D.Vector(Infinity, Infinity);

	return new MathExt.LinAlg2D.Vector(
		(l2.b - l1.b) / (l1.m - l2.m),
		(l1.m*l2.b - l2.m*l1.b) / (l1.m - l2.m)
	);
};

/**
 * Berechnet die Steigung m und den y-Achsenabschnitt b der durch die zwei
 * Punkte p und q definierten Gerade.
 * 
 * @param p		Erster Punkt, durch den die Gerade verläuft
 * @param q		Zweiter Punkt, durch den die Gerade verläuft
 * @result MathExt.LinAlg2D.Line
 */
MathExt.LinAlg2D.calc_line_mb_by_points = function(p, q) {
	if (q.x == p.x)
		return new MathExt.LinAlg2D.Line(Infinity, -Infinity);

	if (q.y == p.y)
		return new MathExt.LinAlg2D.Line(0, p.y);
	
	result = new MathExt.LinAlg2D.Line();
	result.m = (q.y - p.y) / (q.x - p.x);
	result.b = (p.y - result.m*p.x);
	return result;
};