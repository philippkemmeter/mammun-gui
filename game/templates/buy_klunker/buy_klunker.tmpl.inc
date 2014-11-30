<script type="text/javascript">
BuyKlunker = new Object();
<? /**
    * Verweis auf das Fenster, in dem diese Seite hier dargestellt wird.
    */ ?>
BuyKlunker.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];

BuyKlunker.all_countries = <?= Converter::xToJson($country_array) ?>;
BuyKlunker.mobile_price_table = new Object();

<? foreach ($mobile_price_tables as $c => $mobile_price_table) : ?>
BuyKlunker.mobile_price_table['<?= $c ?>'] = new Array();
<? for ($i = 0; $i < count($mobile_price_table); $i++) : ?>
BuyKlunker.mobile_price_table['<?= $c ?>'][<?= $i ?>] = new Object();
BuyKlunker.mobile_price_table['<?= $c ?>'][<?= $i ?>].kkr
	= <?= $mobile_price_table[$i]["kkr"] ?>;
BuyKlunker.mobile_price_table['<?= $c ?>'][<?= $i ?>].price
	= <?= $mobile_price_table[$i]["price"] ?>;
BuyKlunker.mobile_price_table['<?= $c ?>'][<?= $i ?>].currency
	= '<?= $mobile_price_table[$i]["currency"] ?>';
BuyKlunker.mobile_price_table['<?= $c ?>'][<?= $i ?>].dataX =
	'<?= $cryptObj->do_encryption(
		array(
			"product" => $mobile_price_table[$i]['product_id'],
			"country" => $c,
			"buttonid"=> $mobile_price_table[$i]['buttonid']
		)
	)?>'
<? endfor; ?>
<? endforeach; ?>

BuyKlunker.choose_method = function() {
	<? include('choose_method.tmpl.inc') ?>
}

<? /**
    * Für jeden Zeile in der Preistabelle vom normalen Payment eine Funktion
    * erzeugen, die verschlüsselt das entsprechende Produkt aufruft.
    */ ?>
<? $i=0;foreach (PricingTable::$pricing_table_eur_kkr["default"]
					as $eur => $kkr): ?>
BuyKlunker.buy_default<?=$i?> = function(ev) {
	BuyKlunker.current_dataX =
		'<?= $cryptObj->do_encryption(
			array("product" => "Klunker_$kkr", "country" => $country)
		)?>';

	BuyKlunker.choose_method();
}
<? $i++;endforeach;?>

<? /**
    * Diese Funktion wird aufgerufen, wenn der User angegeben hat, was er genau
    * kaufen möchte und mit welcher Zahlungsmethode.
    *
    * @param {'visa', 'master', 'pp', 'mopay'} payment_method
    */ ?>
BuyKlunker.open_external_dialog = function(payment_method) {
	if ((payment_method != 'visa') && (payment_method != 'master') &&
			(payment_method != 'pp') && (payment_method != 'mopay'))
	{
		throw new Error("IllegalArgumentException. "+
				"BuyKlunker.open_external_dialog: first Argument has to be in "+
				"['visa', 'master', 'pp', 'mopay']. '"+payment_method+"' given.");
	}
	<? include('buy_external_dialog.tmpl.inc') ?>
	BuyKlunker.ExternalDialog.feedback_container.set_content('loading...');
	if (payment_method == 'mopay' && false) {
		PureFW.AJAXClientServer.send_request('<?= $_SERVER['PHP_SELF'] ?>'+
			'?dataX='+BuyKlunker.current_dataX+"&"+payment_method+"=true",
			BuyKlunker.open_external_dialog_response_mopay
		);
		return;
	}
	else {
		var width, height, security_url, action_func;
		if(payment_method == 'visa')
		{
			width = 117;
			height = 94;
			security_url = 'ui/icons/labels/buy_klunker/3d_secure/visa_learn_117x94.jpg';
			action_func = function() {
				window.open('http://www.visa.ca/verified/infopane/index.html',
						'InfoPane','height=550,width=450,channelmode=0,dependent=0'+
						',directories=0,fullscreen=0,location=0,menubar=> 0,'+
						'resizable=0,scrollbars=0,status=0,toolbar=0', 'InfoPane');
				}
			BuyKlunker.ExternalDialog.feedback_container.set_width(
					BuyKlunker.ExternalDialog.feedback_container.width - 140);
			BuyKlunker.ExternalDialog.feedback_container.set_x(
					BuyKlunker.ExternalDialog.feedback_container.position.x
					+ 140
			);
		}
		else if(payment_method == 'master')
		{
			width = 120;
			height = 65;
			security_url = 'ui/icons/labels/buy_klunker/3d_secure/sc_learn_120x65.gif';
			action_func = function() {
				window.open('http://www.mastercardbusiness.com/mcbiz/index.jsp?'+
						'template=/orphans&content=securecodepopup', 'LearnMore',
						 '10, 10, width=750, height=550,scrollbars=yes,'+
						 'resizable=yes');
				}
			BuyKlunker.ExternalDialog.feedback_container.set_width(
					BuyKlunker.ExternalDialog.feedback_container.width - 145);
			BuyKlunker.ExternalDialog.feedback_container.set_x(
					BuyKlunker.ExternalDialog.feedback_container.position.x
					+ 145
			);
		}
		else
		{
			action_func = function(){}
		}

		BuyKlunker.ExternalDialog.security_stamp
			= BuyKlunker.ExternalDialog.confirm_box.add_widget
		(
			PureFW.Container,
			25, 60,
			width, height
		);
		BuyKlunker.ExternalDialog.security_stamp.set_bg_img(security_url);
		BuyKlunker.ExternalDialog.security_stamp.add_event_handler('click',
			action_func
		);

		PureFW.AJAXClientServer.send_request('<?= $_SERVER['PHP_SELF'] ?>'+
				'?dataX='+BuyKlunker.current_dataX+"&"+payment_method+"=true",
				BuyKlunker.open_external_dialog_response
		);
	}
}

<? /**
    * Diese Funktion ist die Antwortsabarbeitung von open_external_dialog, da
    * vorherige zunächst beim Server ein paar Sachen abfragen musste.
    *
    * @param string[] response_arr
    */ ?>
BuyKlunker.open_external_dialog_response = function(response_arr)
{
	if (response_arr[0] == '1')
	{
		//alert(	"1: " + response_arr[1] +
		//	"\n 2: " + response_arr[2] +
		//"\n 3: " + response_arr[3] +
		//"\n 4: " + response_arr[4]);
		var browserVer = navigator.appVersion;

		if (browserVer.indexOf('Chrome') == -1)
		{
			alert('kein chrome');
			BuyKlunker.ExternalDialog.open_external_window(
			response_arr[2], response_arr[3], response_arr[4]
		);
		}

		BuyKlunker.ExternalDialog.feedback_container.set_content(
				response_arr[1].replace(/\[HREF\]/,
					'javascript: BuyKlunker.ExternalDialog.'
						+'open_external_window(\''
						+response_arr[2]+'\', '+response_arr[3] +', '
						+response_arr[4]
						+');'
				)
		);
	}
	else {
		throw response_arr;
	}
}

<? /**
    * ! SPEZIELL FÜR BOKU WEGEN DEM WRAPPER !
    * Diese Funktion ist die Antwortsabarbeitung von open_external_dialog,
    * da vorherige zunächst beim Server ein paar Sachen abfragen musste.
    *
    * @param string[] response_arr
    */ ?>
BuyKlunker.open_external_dialog_response_mopay = function(response_arr) {
	if (response_arr[0] == '1') {
		<? include('buy_external_dialog.tmpl.inc') ?>
		BuyKlunker.ExternalDialog.open_external_window_wrapped(
				response_arr[2], response_arr[3], response_arr[4]
		);
		BuyKlunker.ExternalDialog.feedback_container.set_content(
				response_arr[1].replace(/\[HREF\]/,
					'javascript: BuyKlunker.ExternalDialog.'
						+'open_external_window_wrapped(\''
						+response_arr[2]+'\', '+response_arr[3] +', '
						+response_arr[4]
						+');'
				)
		);
	}
	else {
		throw response_arr;
	}
}


<? /**
    * Nur eine Funktion für den Kaufvorgang per mopay. Dafür muss hier das
    * entsprechende Objekt übermittelt werden, das die Details enthält.
    *
    * @param Object mopay_row
    */ ?>
BuyKlunker.buy_per_mopay = function(mopay_row) {
	BuyKlunker.current_dataX = mopay_row.dataX;
	BuyKlunker.open_external_dialog('mopay');
}

<?	/**
	 * Füllt das Dropdown zum Auswählen des Landes mit den entsprechenden
	 * Ländern und Klick-Aktionen
	 */ ?>
BuyKlunker.fill_mobile_country_dropdown = function() {
	for (var i in BuyKlunker.all_countries) {
		BuyKlunker.choose_country_container.add_item(
			BuyKlunker.all_countries[i],
			(function(_country_code) {
				return function(ev) {
					BuyKlunker.fill_mobile_payment_box(_country_code);
				}
			})(i)
		);
	}
	BuyKlunker.choose_country_container.select_by_name(
		BuyKlunker.all_countries['<?= $country ?>']
	);
}

<?	/**
	 * Erzeugt den Inhalt der rechten Box, der Mobile-Payment-Box, in der alle
	 * Kaufoptionen für den SMS-Kauf stehen.
	 */ ?>
BuyKlunker.fill_mobile_payment_box = function(country_code) {
	var n = BuyKlunker.mobile_price_table[country_code].length;

	try {
		BuyKlunker.mopay_row_l.destroy();
		BuyKlunker.mopay_row_l_KKR.destroy();
		BuyKlunker.mopay_row_l_KKR_amount.destroy();
		BuyKlunker.mopay_row_l_EUR.destroy();
		BuyKlunker.mopay_row_l_EUR_amount.destroy();
	}
	catch(e) {}
	BuyKlunker.mopay_row_l = new Array();
	BuyKlunker.mopay_row_l_KKR = new Array();
	BuyKlunker.mopay_row_l_KKR_amount = new Array();
	BuyKlunker.mopay_row_l_EUR = new Array();
	BuyKlunker.mopay_row_l_EUR_amount = new Array();

	for (var i = 0; i < n; i++) {
		var kkr = BuyKlunker.mobile_price_table[country_code][i].kkr;
		var currency = BuyKlunker.mobile_price_table[country_code][i].currency;
		if (currency == "EUR")
			currency = '€';
		var price = BuyKlunker.mobile_price_table[country_code][i].price
			.toFixed(2);
		price.replace('.', ',');


		<? /* linke Hälfte der Zeile: Klunkeranzahl */ ?>
		BuyKlunker.mopay_row_l[i] = BuyKlunker.right_container.add_widget(
			PureFW.Container,
			25, 140 + i*35,
			270, 35
		);
		BuyKlunker.mopay_row_l[i].add_event_handler(
			"click", (function(_o) {
				return function (ev) {
					BuyKlunker.buy_per_mopay(_o);
				}
			})(BuyKlunker.mobile_price_table[country_code][i])
		);

		BuyKlunker.mopay_row_l[i].add_event_handler(
			"mouseover",
			(function(_o) {
				return function (ev) {
					_o.set_bg_img(
						'ui/backgrounds/buy_klunker/bg_selected.png'
					);
				}
			})(BuyKlunker.mopay_row_l[i])
		);

		BuyKlunker.mopay_row_l[i].add_event_handler(
			"mouseout",
			(function(_o) {
				return function (ev) {
					_o.set_bg_img('');
				}
			})(BuyKlunker.mopay_row_l[i])
		);

		<? /* Enthält das Klunker-Symbol */ ?>
		BuyKlunker.mopay_row_l_KKR[i] = BuyKlunker.mopay_row_l[i].add_widget(
			PureFW.Container,
			10, 6,
			23, 21
		);
		BuyKlunker.mopay_row_l_KKR[i].set_bg_img(
			'ui/icons/labels/resources/21/klunker.png'
		);
		<? /* Enthält die Klunkeranzahl */ ?>
		BuyKlunker.mopay_row_l_KKR_amount[i] = BuyKlunker.mopay_row_l[i]
			.add_widget
		(
			PureFW.Container,
			50, 5,
			70, 23
		);
		BuyKlunker.mopay_row_l_KKR_amount[i].set_content(kkr);
		BuyKlunker.mopay_row_l_KKR_amount[i].set_font_size(1.5);
		BuyKlunker.mopay_row_l_KKR_amount[i].set_font_weight(800);
		BuyKlunker.mopay_row_l_KKR_amount[i].set_text_align('right');

		<? /* rechte Hälfte der Zeile: Euromenge */ ?>
		<? /*
		BuyKlunker.mopay_row_r<?=$i?> = new PureFW.Container(
				BuyKlunker.right_container,
				160, <?= 70+$i*40?>,
				135, 40
		);
		BuyKlunker.mopay_row_r<?=$i?>.add_event_handler(
				"click", BuyKlunker.buy_per_mopay<?=$i?>
		);
		 */ ?>


		<? /* Enthält das Euro-Symbol */ ?>
		BuyKlunker.mopay_row_l_EUR[i] = BuyKlunker.mopay_row_l[i].add_widget(
			PureFW.Container,
			120+25, 5,
			40, 23
		);

		BuyKlunker.mopay_row_l_EUR[i].set_content(currency);
		BuyKlunker.mopay_row_l_EUR[i].set_text_align('right');
		BuyKlunker.mopay_row_l_EUR[i].set_font_size(1.5);

		<? /* Enthält die Euromenge */ ?>
		BuyKlunker.mopay_row_l_EUR_amount[i] = BuyKlunker.mopay_row_l[i]
			.add_widget
		(
			PureFW.Container,
			135+50, 5,
			70, 23
		);

		BuyKlunker.mopay_row_l_EUR_amount[i].set_content(price);
		BuyKlunker.mopay_row_l_EUR_amount[i].set_font_size(1.5);
		BuyKlunker.mopay_row_l_EUR_amount[i].set_text_align('right');
	}
}

<? /**
    * Destruktor. Wird aufgerufen,wenn das Fenster geschlossen wird und räumt
    * alles an Objekten hinter sich auf.
    */ ?>
BuyKlunker.destroy = function() {
	BuyKlunker.this_window.remove_event_handler("destroy", BuyKlunker.destroy);
	BuyKlunker.this_window.remove_event_handler("change", BuyKlunker.destroy);
	for (var x in BuyKlunker) {
		<?/* Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		   * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		   * Wir haben hier nicht die Berechtigung dort einzugreifen!*/?>
		if (x == 'this_window')
			continue;
		try {
			BuyKlunker[x].destroy();
		}
		catch (e) {
			delete BuyKlunker[x];
		}
	}
	BuyKlunker = null;
}
<?
/******************************************************************************
 *                  /\     .---.    -----    -     .---.   .  .               *
 *                 /__\    |          |      |     |   |   |`.|               *
 *                /    \   `---'      -      -     `---'   '  '               *
 *                                                                            *
 *                             A  C  T  I  O  N                            PK *
 ******************************************************************************/
?>

<? /* Linker Container. Enthält die Liste für generelles Bezahlen */ ?>
BuyKlunker.left_container = new PureFW.Container(
		BuyKlunker.this_window,
		240,60,
		320,360
);
BuyKlunker.left_container.set_bg_img(
		'ui/backgrounds/buy_klunker/big_box.png'
);
BuyKlunker.left_container.set_content('<?= LNG_BUY_KLUNKER_USING ?>');
BuyKlunker.left_container.set_padding(10);
BuyKlunker.left_container.set_font_size(1.1);

BuyKlunker.visa_img = new PureFW.Image(
	BuyKlunker.left_container.get_content_node(),
	13, 32,
	60, 34,
	'ui/icons/labels/buy_klunker/visa_small.gif'
);
BuyKlunker.master_img = new PureFW.Image(
	BuyKlunker.left_container.get_content_node(),
	BuyKlunker.visa_img.position.x
	+ BuyKlunker.visa_img.width,
	BuyKlunker.visa_img.position.y,
	55, 34,
	'ui/icons/labels/buy_klunker/mastercard_small.gif'
);
BuyKlunker.pay_pal_img = new PureFW.Image(
	BuyKlunker.left_container.get_content_node(),
	BuyKlunker.master_img.position.x
		+ BuyKlunker.master_img.width,
	BuyKlunker.visa_img.position.y,
	61, 34,
	'ui/icons/labels/buy_klunker/paypal_small.gif'
);

BuyKlunker.fest_price_container = new PureFW.Container(
		BuyKlunker.left_container.get_content_node(), 30, 74, 260, 20);
BuyKlunker.fest_price_container.set_content(
	'<?= LNG_BUY_KLUNKER_PRICES_INTERNATIONAL ?>'
);
BuyKlunker.fest_price_container.get_node().style.textAlign = 'center';

BuyKlunker.table_header_container_k1 = new PureFW.Container(
		BuyKlunker.left_container.get_content_node(), 40, 110, 135, 25);
BuyKlunker.table_header_container_k1.set_content('Klunker');
BuyKlunker.table_header_container_k1.set_font_size(1.4);
BuyKlunker.table_header_container_k1.set_font_weight('bold');
BuyKlunker.table_header_container_k1.set_text_align('center');

BuyKlunker.table_header_container_e1 = new PureFW.Container(
		BuyKlunker.left_container.get_content_node(), 175, 110, 135, 25);
BuyKlunker.table_header_container_e1.set_content('Euro');
BuyKlunker.table_header_container_e1.set_font_size(1.4);
BuyKlunker.table_header_container_e1.set_font_weight('bold');
BuyKlunker.table_header_container_e1.set_text_align('center');

<? /**
    * Jetzt wird der linke Container gefüllt mit allen Preismodellen :)
    */ ?>
<? $i=0;foreach (PricingTable::$pricing_table_eur_kkr["default"] as $eur => $kkr): ?>
<? /* linke Hälfte der Zeile: Klunkeranzahl */ ?>
BuyKlunker.default_row_l<?=$i?> = new PureFW.Container(
		BuyKlunker.left_container,
		25, <?= 140+$i*35?>,
		270, 35
);
BuyKlunker.default_row_l<?=$i?>.add_event_handler(
		"click", BuyKlunker.buy_default<?=$i?>
);

BuyKlunker.default_row_l<?=$i?>.add_event_handler(
		"mouseover",
		function() {
			BuyKlunker.default_row_l<?=$i?>.set_bg_img(
				'ui/backgrounds/buy_klunker/bg_selected.png'
			);
		}
);

BuyKlunker.default_row_l<?=$i?>.add_event_handler(
		"mouseout",
		function() {
			BuyKlunker.default_row_l<?=$i?>.set_bg_img('');
		}
);

<? /* Enthält das Klunker-Symbol */ ?>
BuyKlunker.default_row_l_KKR<?=$i?> = new PureFW.Container(
		BuyKlunker.default_row_l<?=$i?>,
		10, 6,
		23, 21
);
BuyKlunker.default_row_l_KKR<?=$i?>.set_bg_img(
	'ui/icons/labels/resources/21/klunker.png'
);
<? /* Enthält die Klunkeranzahl */ ?>
BuyKlunker.default_row_l_KKR_amount<?=$i?> = new PureFW.Container(
		BuyKlunker.default_row_l<?=$i?>,
		50, 5,
		70, 23
);
BuyKlunker.default_row_l_KKR_amount<?=$i?>.set_content('<?=$kkr?>');
BuyKlunker.default_row_l_KKR_amount<?=$i?>.set_font_size(1.5);
BuyKlunker.default_row_l_KKR_amount<?=$i?>.set_font_weight(800);
BuyKlunker.default_row_l_KKR_amount<?=$i?>.set_text_align('right');

<? /* rechte Hälfte der Zeile: Euromenge */ ?>
<? /*
BuyKlunker.default_row_r<?=$i?> = new PureFW.Container(
		BuyKlunker.left_container,
		160, <?= 70+$i*40?>,
		135, 40
);
BuyKlunker.default_row_r<?=$i?>.add_event_handler(
		"click", BuyKlunker.buy_default<?=$i?>
);
 */ ?>

<? /* Enthält das Euro-Symbol */ ?>
BuyKlunker.default_row_l_EUR<?=$i?> = new PureFW.Container(
		BuyKlunker.default_row_l<?=$i?>,
		135+35, 5,
		15, 23
);
BuyKlunker.default_row_l_EUR<?=$i?>.set_content('€');
BuyKlunker.default_row_l_EUR<?=$i?>.set_font_size(1.5);

<? /* Enthält die Euromenge */ ?>
BuyKlunker.default_row_l_EUR_amount<?=$i?> = new PureFW.Container(
		BuyKlunker.default_row_l<?=$i?>,
		135+50, 5,
		70, 23
);
<? /* immer zwei Nachkommastellen */
$eur = sprintf("%01.2f", $eur);
$eur = str_replace('.', ',', $eur);
?>

BuyKlunker.default_row_l_EUR_amount<?=$i?>.set_content('<?=$eur?>');
BuyKlunker.default_row_l_EUR_amount<?=$i?>.set_font_size(1.5);
BuyKlunker.default_row_l_EUR_amount<?=$i?>.set_text_align('right');

<? $i++;endforeach; ?>




<? /* Rechter Container. Enthält die Liste für Handy-Payment-Preise */ ?>
BuyKlunker.right_container = new PureFW.Container(
	BuyKlunker.this_window,
	580,60,
	320,360
);
BuyKlunker.right_container.set_bg_img(
	'ui/backgrounds/buy_klunker/big_box.png'
);

BuyKlunker.right_container.set_font_size(1.1);
BuyKlunker.right_container.get_node().style.overflow = 'visible';
BuyKlunker.right_container_txt = BuyKlunker.right_container.add_widget(
	PureFW.Container,
	10, 10,
	130, 0
);
BuyKlunker.right_container_txt.set_content('<?= LNG_BUY_KLUNKER_MOBILE ?>');

BuyKlunker.right_container.add_widget(
		PureFW.Image,
		145, 8, 163, 49,
		'https://securepay.mopay.com/mopay20/buttons/btn_checkout.gif'
);

BuyKlunker.flexible_price_container = new PureFW.Container(
		BuyKlunker.right_container.get_content_node(),
		44, 75,
		260, 20
);
BuyKlunker.flexible_price_container.set_content(
	'<?= LNG_BUY_KLUNKER_PRICES_NATIONAL ?>'
);

BuyKlunker.choose_country_container = new PureFW.DropMenu(
		BuyKlunker.right_container.get_content_node(), 150, 69, 128, 28);
BuyKlunker.fill_mobile_country_dropdown();


BuyKlunker.table_header_container_k2 = new PureFW.Container(
		BuyKlunker.right_container.get_content_node(), 40, 110, 135, 20);
BuyKlunker.table_header_container_k2.set_content('Klunker');
BuyKlunker.table_header_container_k2.set_font_size(1.5);
BuyKlunker.table_header_container_k2.set_font_weight('bold');
BuyKlunker.table_header_container_k2.set_text_align('center');

BuyKlunker.table_header_container_e2 = new PureFW.Container(
		BuyKlunker.right_container.get_content_node(), 175, 110, 135, 20);
BuyKlunker.table_header_container_e2.set_content('Euro');
BuyKlunker.table_header_container_e2.set_font_size(1.5);
BuyKlunker.table_header_container_e2.set_font_weight('bold');
BuyKlunker.table_header_container_e2.set_text_align('center');

BuyKlunker.fill_mobile_payment_box('<?= $country ?>');

BuyKlunker.this_window.add_event_handler("destroy", BuyKlunker.destroy);
BuyKlunker.this_window.add_event_handler("change", BuyKlunker.destroy);

BuyKlunker.officer = new PureFW.Officer(
		document.body,PureFW.Officer.OFFICERS.salesman);

BuyKlunker.officer.set_position(50,50);
BuyKlunker.officer.set_officer_position(0, 50);
BuyKlunker.officer.set_balloon_position(0, 0);
BuyKlunker.officer.set_text('<?= LNG_KLUNKER_BUY_TEXT ?>');
BuyKlunker.officer.show();
<? /*


 */?>
</script>