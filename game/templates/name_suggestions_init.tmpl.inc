<? Language::get_instance()->load_language_file('explorer_names'); ?>
<script type="text/javascript">
<? for ($i = 1; defined("LNG_EXPL_NAME_M$i"); $i++) : ?>
NameSuggestions.names['m'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPL_NAME_M$i") ?>';
<? endfor; ?>
<? for ($i = 1; defined("LNG_EXPL_NAME_F$i"); $i++) : ?>
NameSuggestions.names['f'][<?= ($i-1) ?>] =
	'<?= constant("LNG_EXPL_NAME_F$i") ?>';
<? endfor; ?>

</script>
