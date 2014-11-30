<script type="text/javascript">
this_window<?=$this_id?> = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];

fancy_error_officer = new PureFW.Officer(document.body);
fancy_error_officer.set_position(10,120);
fancy_error_officer.set_pic_position_offset(-20,0);
fancy_error_officer.set_pic_on_click();
fancy_error_officer.set_text('<?=$error_text?>');

this_window<?=$this_id?>.add_event_handler("destroy", function(){fancy_error_officer.destroy()});
</script>