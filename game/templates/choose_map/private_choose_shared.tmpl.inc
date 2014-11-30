<script type="text/javascript">
<?
/**
 * Es wird ein neues Template ausgesucht :)
 * Diese Funktion erzeugt den nötigen Dialog dafür.
 */
?>
PrivateChooseMapShared = Object();

PrivateChooseMapShared.choose_template_cb = null;


PrivateChooseMapShared.choose_template_dialog = function(
	choosable_map_templates, cb_function)
{
	PrivateChooseMapShared.choose_template_cb = cb_function;

	var conf_box = MammunUI.show_warning(
		530, 352, false, PureFW.ConfirmationBox.NO
	);

	var title_img = conf_box.add_widget
	(
		PureFW.Image,
		30, 20,
		317, 28,
		'ui/elements/texts/<?=$LNG?>/choose_map/choose_template.png'
	);

	var scroller = conf_box.add_widget(
			PureFW.BubbleScroller,
			20,
			title_img.position.y + title_img.height + 15,
			conf_box.width - 40,
			conf_box.height - title_img.position.y - title_img.height - 35
		);

	var n = choosable_map_templates.length;
	for (var i = 0; i < n; i++) {
		scroller.add_item(
			PureFW.ChooseMap.MapTemplatePreview,
			[ 188, 208,
			  choosable_map_templates[i].template_id ],
			(function(_template, _cb) {
				return function () {
					this.set_overlay_bg_img(
						'ui/backgrounds/PolaroidContainer/overlay_bg_ice.png'
					);
					var tmp = this.add_widget_to_overlay(
						PureFW.Container,
						0, 15,
						this.template_label.width,
						20
					);
					tmp.set_text_align('center');
					tmp.add_content(
						'<?= LNG_TEMPLATE_FOR_X_PLAYERS ?>'.
						replace('[PLAYERS]', _template.num_players)
                	);
					tmp = this.add_widget_to_label(
						PureFW.Container,
						0, 0,
						this.widget_label.width,
						20
					);
					tmp.set_text_align('center');
					tmp.add_content(
						'<b><a href="javascript: MammunUI.destroy_warning();'
							+ 'PrivateChooseMapShared.choose_template_cb('
								+_template.template_id+');">'
							+ '<?= LNG_BTN_CHOOSE_THIS_TEMPLATE ?>'
							+ '</b></a>'
					);

					this.add_event_handler("click",
						function(ev) {
							MammunUI.destroy_warning();
							_cb(_template.template_id);
						}
					);
				}
			})(choosable_map_templates[i], cb_function)
		);
	}
}
</script>