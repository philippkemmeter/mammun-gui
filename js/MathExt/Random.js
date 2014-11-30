/**
 * Implementierung mehrerer Pseudozufallsalgorithmen
 */

MathExt.Random = new Object();

/**
 * Implementierung der Halton-Folge.
 * (Siehe Wikipedia für Details)
 * 
 * @param uint index		Index
 * @param prim base			Basis	(Primzahl)
 * @return [0;1]
 */
MathExt.Random.halton = function(index, base) {
	var result = 0;
	var f = 1 / base;
	var i = index;
	while (i > 0) {
		result += f * (i % base);
		i = parseInt(i / base);
		f = f / base;
	}
	return result;
};