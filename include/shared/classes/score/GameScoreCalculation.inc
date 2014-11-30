<?php
require_once(dirname(__FILE__).'/IScoreTypes.inc');
/**
 * Helfer-Namespace. Enthält Funktionen zum Berechnen der Punktzahl eines
 * Spielers
 *
 * @author Philipp Kemmeter
 */
class GameScoreCalculation implements IScoreTypes {
	/**
	 * Konstruktor ist private, da GameScoreCalculation rein statisch(namespace)
	 */
	private function __construct() {

	}

	/**
	 * Berechnet die Stufe, die ein Spieler mit der angegebenen Punktzahl hat.
	 *
	 * Man kann ein Level tippen mithilfe von <code>$level_guess_min</code>
	 * und <code>$level_guess_max</code> wenn man glaubt ungefähr zu wissen,
	 * welches Ergebnis man erwartet. Dann werden zunächst die angegebenen
	 * Level im Cache durchsucht, ob der Tipp stimmen kann, und falls entweder
	 * kein Cache existiert oder der Tipp falsch ist, das Level erst dann
	 * berechnet. Ein guter Tipp spart also deutlich Rechenzeit.
	 *
	 * @param ufloat $score
	 * @param uint $level_guess_min
	 * @param uint $level_guess_max
	 * @return uint
	 */
	public static function calc_level_by_score($score, $level_guess_min=0,
		$level_guess_max=0)
	{
		if ($score<1)
			$score=1;
		for ($i = $level_guess_min; $i < $level_guess_max; $i++) {
			if (!isset($_SESSION['Shared_Score_GameScoreCalculation']
											['score_by_level_cache'][$i]) ||
				!isset($_SESSION['Shared_Score_GameScoreCalculation']
											['score_by_level_cache'][$i+1])) {
				continue;
			}
			if (($score >= $_SESSION['Shared_Score_GameScoreCalculation']
											['score_by_level_cache'][$i]) &&
				($score < $_SESSION['Shared_Score_GameScoreCalculation']
											['score_by_level_cache'][$i+1]))
			{
				return $i;
			}
		}
		/**
		 * Das innere ceil sorgt nur dafür, dass die Punkte in 10er Schritten
		 * verglichen werden (s. auch calc_score_by_level). Dann die Magic
		 * Function und das Ergebnis wird dann nochmal aufgerundet, damit es
		 * keine Kommas gibt (äußeres ceil)
		 */
		return ceil(pow(ceil($score/10)*10,0.7)/20);
	}

	/**
	 * Berechnet die Punktzahl, die benötigt wird, um die angegebene Stufe zu
	 * erreichen.
	 *
	 * @param uint $level
	 * @return ufloat
	 */
	public static function calc_score_by_level($level) {
		if (!isset($_SESSION['Shared_Score_GameScoreCalculation']
											['score_by_level_cache'][$level]))
		{
			$_SESSION['Shared_Score_GameScoreCalculation']
											['score_by_level_cache'][$level]
			/**
			 * Das ist die Umkehrfunktion von der in calc_level_by_score. Das
			 * floor(..) über das ganze dient zur 10er-Stufen Berechnung. Der
			 * Genauigkeit wegen, wird das ceil trotzdem beibehalten, obwohl
			 * man es wahrscheinlich nicht braucht.
			 */
				= floor(ceil(pow(($level-1)*20, 1/0.7))/10)*10;
		}
		return $_SESSION['Shared_Score_GameScoreCalculation']
											['score_by_level_cache'][$level];
	}

	/**
	 * Berechnet, wie viele Punkte für die entsprechende Aktion, welche durch
	 * $type und $amount spezifiziert ist, hinzugefügt würden.
	 *
	 * @param uint $amount
	 * @param uint $type
	 * @return uint
	 */
	public static function calc_score($amount, $type=self::TYPE_UNKNOWN) {
		switch ($type) {
			case self::TYPE_RESSOURCE_GAIN:
				$amount = 0;
				break;
			case self::TYPE_RESSOURCE_USE:
				$amount = ceil($amount/250);
				break;
			case self::TYPE_SCIENCE_RESULT:
				$amount = ceil($amount);
				break;
			case self::TYPE_MEDAL:
				$amount *= 25;
		}
		return $amount;
	}
}
?>