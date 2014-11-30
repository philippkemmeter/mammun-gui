uploaded_files<?=$this_id?> = new Array();

upload_done<?=$this_id?> = function(success, msg, additional) {
	if (success) {
		var new_div = document.createElement('div');
		new_div.id = msg+'<?=$this_id?>';
		new_div.innerHTML = msg+' ('+additional+' Bytes) '
			+ '<a href="javascript: remove_attachment<?=$this_id?>'
			+ '(\''+ msg +'\');">[X]</a>';
		document.getElemById('uploaded_file_list<?=$this_id?>').
			appendChild(new_div);

		document.setElemInnerHTML('bug_feedback<?= $this_id?>', '');
		uploaded_files<?=$this_id?>.push(msg);
	}
	else {
		document.setElemInnerHTML('bug_feedback<?= $this_id?>',
			'<p style="color: red">'+msg+'</p>');
	}
};

remove_callback<?=$this_id?> = function(response_arr) {
	if (response_arr[0] == 1) {
		try {
			uploaded_files<?=$this_id?>.remove(response_arr[1]);
			document.getElemById('uploaded_file_list<?=$this_id?>').removeChild(
				document.getElemById(response_arr[1] + '<?=$this_id?>')
			);
		}
		catch(e) {}
	}
};

remove_attachment<?=$this_id?> = function(name) {
	PureFW.AJAXClientServer.send_request(
		"bugreport.php?del_attach="+name,
		remove_callback<?=$this_id?>
	);
};

abort<?=$this_id?> = function() {
	var n = uploaded_files<?=$this_id?>.length;
	for (var i = 0; i < n; i++) {
		PureFW.AJAXClientServer.send_request(
			"bugreport.php?del_attach="+uploaded_files<?=$this_id?>[i],
			remove_callback<?=$this_id?>
		);
	}
	UIButtons.toggle_bugreport_window();
};

form_submit<?=$this_id?> = function(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){
	}
	ev.preventDefault();

	/**resolution **/
	var screenWidth = screen.width;
	var screenHeight = screen.height;
	document.forms.bugreport<?=$this_id?>.resolution.value=screenWidth+'x'+screenHeight;
	document.forms.bugreport<?=$this_id?>.user_id.value='<?= $cryptObj
		->do_encryption(array('user_id' => User::get_current_user()->get_id()))?>';

	/**window_size **/
	function Fensterweite ()
	{
  		if (window.innerWidth)
  		{
    		return window.innerWidth;
  		}
  		else if (document.body && document.body.offsetWidth)
  		{
    		return document.body.offsetWidth;
  		}
  		else
  		{
    		return 0;
  		}
	}

	function Fensterhoehe ()
	{
  		if (window.innerHeight)
  		{
    		return window.innerHeight;
  		}
  		else if (document.body && document.body.offsetHeight)
  		{
    		return document.body.offsetHeight;
  		}
  		else
  		{
    		return 0;
  		}
	}
	document.forms.bugreport<?=$this_id?>.window_size.value =
		Fensterweite() + 'x' + Fensterhoehe();

	/** browser **/
	document.forms.bugreport<?=$this_id?>.browser.value =
		navigator.appCodeName + " " + navigator.appName + " "
		+ navigator.appVersion;

	/** os **/
	document.forms.bugreport<?=$this_id?>.os.value = navigator.platform;

	PureFW.AJAXClientServer.send_request("bugreport.php", bug_callback<?= $this_id ?>,
		form_elems_to_param_string(document.forms.bugreport<?=$this_id?>.elements));
}

stop_prop<?=$this_id?> = function (ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){
	}
	ev.stopPropagation();
}

bug_callback<?= $this_id ?> = function(response_arr) {
	if (response_arr[0] == '1') {
		document.setElemInnerHTML('bug_feedback<?= $this_id?>', response_arr[1]);
		document.forms.bugreport<?=$this_id?>.reset();
		document.setElemInnerHTML('uploaded_file_list<?=$this_id?>', '');
		document.forms.bugreport_upload<?=$this_id?>.reset();
	}
	else {
		document.setElemInnerHTML('bug_feedback<?= $this_id?>',
			'ERROR:'+response_arr);
	}
}