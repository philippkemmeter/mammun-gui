<? Language::get_instance()->load_language_file('explorer_texts'); ?>
<script type="text/javascript">
<? /** Meine Mammuns im Kampf */ ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_LOWHPFIGHT$i"); $i++) : ?>
MammunSpeech.explt['my']['fight']['lowhp'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_LOWHPFIGHT$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_CLUBFIGHT$i"); $i++) : ?>
MammunSpeech.explt['my']['fight']['club'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_CLUBFIGHT$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_BLADEFIGHT$i"); $i++) : ?>
MammunSpeech.explt['my']['fight']['blade'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_BLADEFIGHT$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_JAVELINFIGHT$i"); $i++) : ?>
MammunSpeech.explt['my']['fight']['javelin'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_JAVELINFIGHT$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_BOWFIGHT$i"); $i++) : ?>
MammunSpeech.explt['my']['fight']['bow'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_BOWFIGHT$i") ?>';
<? endfor; ?>

<? /** Meine Mammuns nicht im Kampf */ ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_LOWHP$i"); $i++) : ?>
MammunSpeech.explt['my']['nofight']['lowhp'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_LOWHP$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_NAKED$i"); $i++) : ?>
MammunSpeech.explt['my']['nofight']['naked'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_NAKED$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_CLUB$i"); $i++) : ?>
MammunSpeech.explt['my']['nofight']['club'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_CLUB$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_BLADE$i"); $i++) : ?>
MammunSpeech.explt['my']['nofight']['blade'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_BLADE$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_JAVELIN$i"); $i++) : ?>
MammunSpeech.explt['my']['nofight']['javelin'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_JAVELIN$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_MY_BOW$i"); $i++) : ?>
MammunSpeech.explt['my']['nofight']['bow'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_MY_BOW$i") ?>';
<? endfor; ?>


<? /** Fremde Mammuns im Kampf */ ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_CLUBFIGHT$i"); $i++) : ?>
MammunSpeech.explt['their']['fight']['club'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_CLUBFIGHT$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_BLADEFIGHT$i"); $i++) : ?>
MammunSpeech.explt['their']['fight']['blade'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_BLADEFIGHT$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_JAVELINFIGHT$i"); $i++) : ?>
MammunSpeech.explt['their']['fight']['javelin'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_JAVELINFIGHT$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_BOWFIGHT$i"); $i++) : ?>
MammunSpeech.explt['their']['fight']['bow'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_BOWFIGHT$i") ?>';
<? endfor; ?>

<? /** Fremde Mammuns nicht im Kampf */ ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_NAKED$i"); $i++) : ?>
MammunSpeech.explt['their']['nofight']['naked'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_NAKED$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_CLUB$i"); $i++) : ?>
MammunSpeech.explt['their']['nofight']['club'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_CLUB$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_BLADE$i"); $i++) : ?>
MammunSpeech.explt['their']['nofight']['blade'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_BLADE$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_JAVELIN$i"); $i++) : ?>
MammunSpeech.explt['their']['nofight']['javelin'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_JAVELIN$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPLT_THEIR_BOW$i"); $i++) : ?>
MammunSpeech.explt['their']['nofight']['bow'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPLT_THEIR_BOW$i") ?>';
<? endfor; ?>

</script>
