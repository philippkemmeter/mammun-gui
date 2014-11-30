<?php

/**
 * Dieser Klasse bietet Zugriff auf die Schatzkiste eines (ggf. anderen)
 * Spielers und den darin enthaltenen Items.
 *
 * @author Philipp Kemmeter
 */
class TreasureChest {
	private $uid;
	static private $INSTANCE = array();

 	/**
	 * Diese Statische Funtion als Konstruktor benutzen !
	 *
	 * @param uint $uid
	 * @return TreasureChest
	 */
 	public static function get_instance($uid) {
 		if (!isset(self::$INSTANCE[$uid])) {
 			self::$INSTANCE[$uid] = new self($uid);
 		}
 		return self::$INSTANCE[$uid];
 	}

 	private function __construct($uid) {
 		$this->uid = $uid;
 	}

 	public function get_items() {
		$all_items = Converter::stdClassToArray(
			Server::get_instance(CONF_PAYMENT_SERVER_URL,
				CONF_MAMMUN_AUTH_SECRET)->get_my_items(
					$this->uid, 0
				)
		);
		$restrictions = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_item_restrictions(User::get_current_user()->get_id(),0);
		$statistics = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_item_statistics(0,0);

		/**
		 * Die erhaltenen Items für die Ausgabe konvertieren. Regeln sind:
		 * 1. Keine User-IDs nur Nicks
		 * 2. Timestamps sollen menschenlesbar sein
		 * 3. Wenn es die Schatzkiste eines anderen ist, dann bestimmte Infos filtern
		 * 4. Name und Beschreibung der Items hinzufügen
		 */
		$has_items = false;
		foreach ($all_items as $type => $items) {
			foreach ($items as $i => $data) {
				$has_items = true;
				foreach ($data->offers as $j => $offer) {
					/**
					 * Wir wollen keine User-IDs, nur Nicks
					 */
					$all_items[$type][$i]->offers[$j]->buyer_nick
						= User::get_instance_by_id($offer->buyer_id)->get_nick();
					unset($all_items[$type][$i]->offers[$j]->buyer_id);
					$all_items[$type][$i]->offers[$j]->t_stamp =
						date("d.m.Y, H:i", $offer->t_stamp);
				}

				if ($this->uid != User::get_current_user()->get_id()) {
					/**
					 * Nur "mir" (Verkäufer) den Einkaufspreis zeigen.
					 */
					unset($all_items[$type][$i]->shop_price);

					/**
					 * Gelockte Items vom Marktplatz nehmen (nicht anzeigen)
					 */
					$all_items[$type][$i]->amount_mp -=
						$all_items[$type][$i]->locked;


		//			/**
		//			 * Nur die anzeigen, die im Schaufenster sind.
		//			 */
		//			if ($all_items[$type][$i]->amount_mp <= 0) {
		//				unset($all_items[$type][$i]);
		//				continue;
		//			}
		//			else {
		//				/**
		//				 * Die Anzahl zuhause gleich Nullen, dass man die gar nicht erst
		//				 * ausgeben kann.
		//				 */
		//				$all_items[$type][$i]->amount = 0;
		//			}

				}
				if (isset($statistics->$type->{$data->id})) {
					$all_items[$type][$i]->rareness_factor
						= ($statistics->$type->{$data->id}->rareness_factor) / 10;
				}
				else {
					$all_items[$type][$i]->rareness_factor = 0;
				}
				if (isset($restrictions->$type->{$data->id})) {
					$all_items[$type][$i]->xp =
						(int)$restrictions->$type->{$data->id}->usr_level;
					$all_items[$type][$i]->may_buy =
						(int)$restrictions->$type->{$data->id}->may_buy_from_shop;
				}
				else {
					$all_items[$type][$i]->xp = 0;
					$all_items[$type][$i]->may_buy = 1;
				}
				$all_items[$type][$i]->name
					= ItemConstants::$ITEM_NAMES_2[$type][$data->id];

				$all_items[$type][$i]->desc
					= ItemConstants::$ITEM_DESCS[$type][$data->id];
			}
		}
		return $all_items;
 	}

 	/**
 	 * Der aktuelle Spieler besucht diese Schatzkiste
 	 */
 	public function visit() {
 		Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
 			->visit_chest(User::get_current_user()->get_id(), $this->uid);
 	}

 	/**
 	 * Der aktuelle Spieler beendet den Besuch der Schatzkiste
 	 */
 	public function unvisit() {
 		Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
 			->unvisit_chest(User::get_current_user()->get_id(), $this->uid);
 	}

 	/**
 	 * Gibt zurück, wer alles die Schatzkiste besucht.
 	 *
 	 * @return uint[]
 	 */
 	public function get_visitors() {
 		return Server::get_instance(CONF_PAYMENT_SERVER_URL,
 			CONF_MAMMUN_AUTH_SECRET)
 				->get_visitors_of_chest($this->uid);
 	}
}