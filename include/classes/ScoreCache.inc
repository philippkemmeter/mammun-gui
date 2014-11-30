<?php
/**
 * Namespace zum Erhalten der Punkte der Spieler.
 *
 * Punkte können auch gesetzt werden, was jedoch nicht an die Engine propagiert
 * wird, sondern nur den hiesigen Cache beeinflusst. Das kann nützlich sein,
 * wenn die Punktzahl durch RPC-Aufrufe bekannt ist.
 *
 * @author Philipp Kemmeter
 */
class ScoreCache {	// Namespace
	private function __construct(){}


	/**
	 * Gibt die gecachete Kartenpunktzahl des Spielers mit der ID $uid zurück.
	 * Ist keine Punktzahl bislang gespeichert worden in dieser Session, wird
	 * von der Engine die aktuelle erfragt. Wird $forced gesetzt, wird immer
	 * die aktuelle vom Server erfragt und die gecachete überschrieben.
	 *
	 * @param uint $world_id
	 * @param uint $map_id
	 * @param uint $uid
	 * @param bool $force
	 * @return float
	 */
	public static function get_map_score($world_id, $map_id, $uid, $forced=false) {
		if (!isset($_SESSION['Score']['map_scores'][$world_id][$map_id][$uid])
			|| $forced)
		{
			$_SESSION['Score']['map_scores'][$world_id][$map_id][$uid] =
				Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_map_score($world_id, $map_id, $uid);
//			error_log("Score for User $uid on Map $map_id fetched from engine.");
		}
		return $_SESSION['Score']['map_scores'][$world_id][$map_id][$uid];
	}

	/**
	 * Leert den angegebenen Cache für die Kartenpunktzahlen.
	 *
	 * @param uint $world_id
	 * @param uint $map_id
	 * @param uint $uid
	 */
	public static function clear_cache_map ($world_id=false, $map_id=false,
		$uid=false)
	{
		if ($world_id) {
			if ($map_id) {
				if ($uid) {
					unset($_SESSION['Score']['map_scores'][$world_id][$map_id]
						[$uid]);
				}
				else {
					unset($_SESSION['Score']['map_scores'][$world_id][$map_id]);
				}
			}
			else {
				unset($_SESSION['Score']['map_scores'][$world_id]);
			}
		}
		else {
			unset ($_SESSION['Score']['map_scores']);
		}
	}

	/**
	 * Gibt die gecachete Liga-Punktzahl des Spielers mit der ID $uid zurück.
	 *
	 * Ist keine Punktzahl bislang gespeichert worden in dieser Session, wird
	 * von der Engine die aktuelle erfragt. Wird $forced gesetzt, wird immer
	 * die aktuelle vom Server erfragt und die gecachete überschrieben.
	 *
	 * @param uint $league_id
	 * @param uint $uid
	 * @param bool $force
	 * @return float
	 */
	public static function get_league_score($league_id, $uid, $forced=false) {
		if (!isset($_SESSION['Score']['league_scores'][$league_id][$uid])
			|| $forced)
		{
			$_SESSION['Score']['league_scores'][$league_id][$uid] =
				Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_league_score($league_id, $uid);
//			error_log(
//				"Score for User $uid in League $league_id fetched from engine."
//			);
		}
		return $_SESSION['Score']['league_scores'][$league_id][$uid];
	}


	/**
	 * Leert den angegebenen Cache für die Kartenpunktzahlen.
	 *
	 * @param uint $league_id
	 * @param uint $uid
	 */
	public static function clear_cache_league ($league_id=false, $uid=false) {
		if ($league_id) {
			if ($uid) {
				unset($_SESSION['Score']['league_scores'][$league_id][$uid]);
			}
			else {
				unset($_SESSION['Score']['league_scores'][$league_id]);
			}
		}
		else {
			unset($_SESSION['Score']['league_scores']);
		}
	}

	/**
	 * Überschreibt die Kartenpunktzahl des Spielers mit der ID $uid mit $score
	 * im Cache. Dieses Setzen wird nicht an die Score-Engine weitergegeben und
	 * somit nur für diese Session so gehalten.
	 *
	 * @param uint $world_id
	 * @param uint $map_id
	 * @param uint $uid
	 * @param float $score
	 */
	public static function set_map_score($world_id, $map_id, $uid, $score) {
		$_SESSION['Score']['map_scores'][$world_id][$map_id][$uid] = $score;
	}

	/**
	 * Überschreibt die Liga-Punktzahl des Spielers mit der ID $uid mit $score
	 * im Cache. Dieses Setzen wird nicht an die Score-Engine weitergegeben und
	 * somit nur für diese Session so gehalten.
	 *
	 * @param uint $league_id
	 * @param uint $uid
	 * @param float $score
	 */
	public static function set_league_score($league_id, $uid, $score) {
		$_SESSION['Score']['league_scores'][$league_id][$uid] = $score;
	}
}
?>