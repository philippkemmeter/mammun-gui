<?php
class PricingTable {

	/**
	 * Gibt für jede Zahlungsmethode an, wieviele Klunker wieviel Euro kosten.
	 * Beispielsweise ist $pricing_table["CC"][100] == 1, was bedeutet, dass
	 * bei Kreditkartenzahlung 100 Klunker 1 Euro kosten.
	 * "Default" ist für alle nicht spezifizierten.
	 */
	public static $pricing_table_kkr_eur = array(
		"default"	=>	array (
			"100"	=>	1,
			"320"	=>	3,
			"550"	=>	5,
			"1250"	=>	10,
			"2900"	=>	20,
			"8000"	=>	50
		),
		"mobile"	=>	array(
			"reference" => array (	// gewünschtee Richtwerte
				"70"	=>	0.89,
				"155"	=>	1.89,
				"250"	=>	2.89,
				"360"	=>	3.89,
				"490"	=>	4.89
			),
			"de"	=>	array(
				"75"	=>	0.99,
				"250"	=>	2.99,
				"490"	=>	4.99,
				"1100"	=>	9.99
			),
			"at"	=>	array(
				"75"	=>	1,
				"250"	=>	3,
				"490"	=>	5,
				"1100"	=>	10
			),
			"ch"	=>	array(	// in CHF
				"85"	=>	1.5,		// ca. 1.11 EUR
				"350"	=>	5			// ca. 3.7 EUR
			)
		)
	);

	/**
	 * Gibt für jede Zahlungsmethode an, wieviele Klunker wieviel Euro kosten.
	 * Beispielsweise ist $pricing_table["CC"][100] == 1, was bedeutet, dass
	 * bei Kreditkartenzahlung 100 Klunker 1 Euro kosten.
	 * "Default" ist für alle nicht spezifizierten.
	 */
	public static $pricing_table_eur_kkr = array(
		"default"	=>	array (
			"1"		=>	100,
			"3"		=>	320,
			"5"		=>	550,
			"10"	=>	1250,
			"20"	=>	2900,
			"50"	=>	8000
		),
		"mobile"	=>	array(
			"reference"	=>	array (
				"0.89"	=>	70,
				"1.89"	=>	155,
				"2.89"	=>	250,
				"3.89"	=>	360,
				"4.89"	=>	490
			),
			"de"	=>	array(
				"0.99"	=>	75,
				"2.99"	=>	250,
				"4.99"	=>	490,
				"9.99"	=>	1100
			),
			"at"	=>	array(
				"1"		=>	75,
				"3"		=>	250,
				"5"		=>	490,
				"10"	=>	1100
			),
			"ch"	=>	array(	// in CHF
				"1.5"	=>	85,
				"5"		=>	350
			)
		)
	);

	public static $currencies = array(
		"default"	=>	"EUR",
		"mobile"	=>	array(
			"reference"	=>	"EUR",
			"de"		=>	"EUR",
			"at"		=>	"EUR",
			"ch"		=>	"CHF"
		)
	);

	/**
	 * Gibt die Klunkeranzahl zurück, die der User für eine freigewählte
	 * EUR-Anzahl beim Mobilfunk-SMS-Bezahlen bekommt.
	 * @param ufloat $eur
	 * @return uint
	 */
	public static function get_variable_mobile_kkr($eur) {
		$org_eur = 0;
		foreach (self::$pricing_table_kkr_eur["mobile"] as $euro_value) {
			if ($euro_value > $eur)
				break;
			$org_eur = $euro_value;
		}
		if (!$org_eur) {
			throw new Exception("get_variable_mobile_kkr: $eur <-> $org_eur!");
		}
		$kkr = self::$pricing_table_eur_kkr["mobile"][(string)$org_eur];
		return floor($kkr * ($eur/$org_eur) / 10)*10;
	}

}
?>
