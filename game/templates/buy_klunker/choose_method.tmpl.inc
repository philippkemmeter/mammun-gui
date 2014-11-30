BuyKlunker.ChooseMethod = new Object();

BuyKlunker.ChooseMethod.destroy = function() {
	for (var x in BuyKlunker.ChooseMethod) {
		<?/* Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		   * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		   * Wir haben hier nicht die Berechtigung dort einzugreifen!*/?>
		if (x == 'this_window')
			continue;
		try {
			BuyKlunker.ChooseMethod[x].destroy();
		}
		catch (e) {
			delete BuyKlunker.ChooseMethod[x];
		}
	}
	delete BuyKlunker.ChooseMethod;
}

<? /**
    * Die Confirmation-Box, welche jetzt aufgeht, um die Auswahl anzuzeigen
    */ ?>

BuyKlunker.ChooseMethod.confirm_box = MammunUI.create_conf_box(
		600, 250
);
BuyKlunker.ChooseMethod.confirm_box.add_event_handler(
	"confirm",
	function(ev) {
		if (BuyKlunker.ChooseMethod.visa_radio.is_checked()) {
			BuyKlunker.open_external_dialog('visa');
		}
		else if (BuyKlunker.ChooseMethod.mastercard_radio.is_checked()) {
			BuyKlunker.open_external_dialog('master');
		}
		else if (BuyKlunker.ChooseMethod.paypal_radio.is_checked()) {
			BuyKlunker.open_external_dialog('pp');
		}
	}
);

<? /**
    *
    * Der Inhalt der Confirmation-Box folgt jetzt!
    *
    */ ?>


<? /**
    * Die Überschrift "Bitte wähle eine Zahlungsmöglichkeit"
    */ ?>
BuyKlunker.ChooseMethod.topic_container = new PureFW.Container(
		BuyKlunker.ChooseMethod.confirm_box,
		0,
		25,
		BuyKlunker.ChooseMethod.confirm_box.width,
		40
);
BuyKlunker.ChooseMethod.topic_container.set_font_size(1.2);
BuyKlunker.ChooseMethod.topic_container.set_text_align('center');
BuyKlunker.ChooseMethod.topic_container.set_content(
		'<?= LNG_BUY_KLUNKER_CHOOSE_METHOD ?>:'
);

<? /**
	*
	*
	* VISA
	*
	*
    * Der Gruppierungsrahmen, in dem das VISA Logo und ein RadioButton zu sehen
    * ist, um VISA zu wählen
    */ ?>
BuyKlunker.ChooseMethod.visa_frame = new PureFW.Container(
		BuyKlunker.ChooseMethod.confirm_box,
		(BuyKlunker.ChooseMethod.confirm_box.width >> 1)-250,
		70,
		148,
		113
);
BuyKlunker.ChooseMethod.visa_frame.get_node().style.overflow = 'visible';
BuyKlunker.ChooseMethod.visa_frame.set_bg_img(
	'ui/backgrounds/grouping_frames/148x113.png'
);
<? /**
    * Das VISA-Logo
    */ ?>
BuyKlunker.ChooseMethod.visa_logo = new PureFW.Image(
	BuyKlunker.ChooseMethod.visa_frame,
	(BuyKlunker.ChooseMethod.visa_frame.width - 115) >> 1,
	10+15,
	115,
	65,
	'ui/icons/labels/buy_klunker/visa_big.gif'
);
<? /**
    * Der Radio-Button für die Auswahl VISA
    */ ?>
BuyKlunker.ChooseMethod.visa_radio = new PureFW.RadioButton(
	BuyKlunker.ChooseMethod.visa_frame,
	(BuyKlunker.ChooseMethod.visa_frame.width - 28) >> 1,
	BuyKlunker.ChooseMethod.visa_logo.position.y
		+ BuyKlunker.ChooseMethod.visa_logo.height + 7,
	28, 28,
	'ui/elements/form/input/radio_toggle.png'
);


<? /**
	*
	*
	* MASTERCARD
	*
	*
    * Der Gruppierungsrahmen, in dem das MASTERCARD Logo und ein RadioButton zu sehen
    * ist, um MASTERCARD zu wählen
    */ ?>
BuyKlunker.ChooseMethod.mastercard_frame = new PureFW.Container(
		BuyKlunker.ChooseMethod.confirm_box,
		(BuyKlunker.ChooseMethod.confirm_box.width - 148) >> 1,
		BuyKlunker.ChooseMethod.visa_frame.position.y,
		148,
		113
);
BuyKlunker.ChooseMethod.mastercard_frame.get_node().style.overflow = 'visible';
BuyKlunker.ChooseMethod.mastercard_frame.set_bg_img(
	'ui/backgrounds/grouping_frames/148x113.png'
);
<? /**
    * Das MASTERCARD-Logo
    */ ?>
BuyKlunker.ChooseMethod.mastercard_logo = new PureFW.Image(
	BuyKlunker.ChooseMethod.mastercard_frame,
	(BuyKlunker.ChooseMethod.mastercard_frame.width - 115) >> 1,
	10+15,
	115,
	65,
	'ui/icons/labels/buy_klunker/mastercard_big.gif'
);
<? /**
    * Der Radio-Button für die Auswahl MASTERCARD
    */ ?>
BuyKlunker.ChooseMethod.mastercard_radio = new PureFW.RadioButton(
	BuyKlunker.ChooseMethod.mastercard_frame,
	(BuyKlunker.ChooseMethod.mastercard_frame.width - 28) >> 1,
	BuyKlunker.ChooseMethod.visa_radio.position.y,
	28, 28,
	'ui/elements/form/input/radio_toggle.png'
);
BuyKlunker.ChooseMethod.mastercard_radio.link_with_radio_button(
	BuyKlunker.ChooseMethod.visa_radio
);

<? /**
	*
	*
	* PAYPAL
	*
	*
    * Der Gruppierungsrahmen, in dem das PAYPAL Logo und ein RadioButton zu sehen
    * ist, um PAYPAL zu wählen
    */ ?>
BuyKlunker.ChooseMethod.paypal_frame = new PureFW.Container(
		BuyKlunker.ChooseMethod.confirm_box,
		(BuyKlunker.ChooseMethod.confirm_box.width >> 1)+250-148,
		BuyKlunker.ChooseMethod.visa_frame.position.y,
		148,
		113
);
BuyKlunker.ChooseMethod.paypal_frame.get_node().style.overflow = 'visible';
BuyKlunker.ChooseMethod.paypal_frame.set_bg_img(
	'ui/backgrounds/grouping_frames/148x113.png'
);
<? /**
    * Das PAYPAL-Logo
    */ ?>
BuyKlunker.ChooseMethod.paypal_logo = new PureFW.Image(
	BuyKlunker.ChooseMethod.paypal_frame,
	(BuyKlunker.ChooseMethod.paypal_frame.width - 115) >> 1,
	10+15,
	115,
	65,
	'ui/icons/labels/buy_klunker/paypal_big.gif'
);
<? /**
    * Der Radio-Button für die Auswahl PAYPAL
    */ ?>
BuyKlunker.ChooseMethod.paypal_radio = new PureFW.RadioButton(
	BuyKlunker.ChooseMethod.paypal_frame,
	(BuyKlunker.ChooseMethod.paypal_frame.width - 28) >> 1,
	BuyKlunker.ChooseMethod.visa_radio.position.y,
	28, 28,
	'ui/elements/form/input/radio_toggle.png'
);
BuyKlunker.ChooseMethod.paypal_radio.link_with_radio_button(
	BuyKlunker.ChooseMethod.mastercard_radio
);
BuyKlunker.ChooseMethod.visa_radio.link_with_radio_button(
	BuyKlunker.ChooseMethod.paypal_radio
);