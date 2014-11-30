/**
 * @author	Marcel Werk
 * @copyright	2001-2007 WoltLab GmbH
 * @license	GNU Lesser General Public License <http://opensource.org/licenses/lgpl-license.php>
 */
// define some global constants
// browser types
var USER_AGENT 		= navigator.userAgent.toLowerCase();
var IS_OPERA 		= (USER_AGENT.indexOf('opera') != -1);
var IS_SAFARI 		= ((USER_AGENT.indexOf('applewebkit') != -1) || (navigator.vendor == 'Apple Computer, Inc.'));
var IS_IE		= ((USER_AGENT.indexOf('msie') != -1) && !IS_OPERA && !IS_SAFARI);
var IS_IE7		= false;
var IS_IE6		= false;
if (IS_IE) {
	if (!IS_OPERA && window.XMLHttpRequest) IS_IE7 = true;
	else IS_IE6 = true;
}
var IS_MOZILLA		= ((navigator.product == 'Gecko') && !IS_SAFARI);
var IS_KONQUEROR	= (USER_AGENT.indexOf('konqueror') != -1);

/**
 * Handle multiple onload events
 * usage: onloadEvents.push(function() {...});
 */
var onloadEvents = new Array();
window.onload = function() {
	for (var i = 0; i < onloadEvents.length; i++)
		onloadEvents[i]();
}

/**
 * Enables options in form dialog.
 */
function enableOptions() {
	for (var i = 0; i < arguments.length; i++) {
		var element = document.getElementById(arguments[i] + 'Div');
			
		if (element) {
			// enable form elements 
			enableFormElements(element, true);
			
			// change class
			var className = element.className;
			className = className.replace(/ ?disabled/, '');
			className = className.replace(/disabled ?/, '');
			
			element.className = className;
		}
	}
	
	return true;
}

/**
 * Enables form elements of an option.
 */
function enableFormElements(parent, enable) {
	if (!parent.childNodes) {
		return;
	}
	
	for (var i = 0; i < parent.childNodes.length; i++) {
		var name = parent.childNodes[i].nodeName;
		if (name) name = name.toLowerCase();
		if (name == 'textarea' || name == 'input' || name == 'select') {
			if (name == 'select' || parent.childNodes[i].type == 'checkbox' || parent.childNodes[i].type == 'radio') {
				parent.childNodes[i].disabled = !enable;
			}
			else {
				if (enable) parent.childNodes[i].removeAttribute('readonly');
				else parent.childNodes[i].setAttribute('readonly', true);
			}
		}
		enableFormElements(parent.childNodes[i], enable);
	}
}

/**
 * Disables options in form dialog.
 */
function disableOptions() {
	for (var i = 0; i < arguments.length; i++) {
		var element = document.getElementById(arguments[i] + 'Div');
		
		if (element) {
			// disable form elements 
			enableFormElements(element, false);
			
			// change class
			var className = element.className;
			
			if (className.indexOf('disabled') == -1) {
				if (className != '') {
					className += ' ';
				}
				className += 'disabled';
				element.className = className;
			}
		}
	}
	
	return true;
}

/**
 * Opens or closes a html list of elements.
 *
 * @param	string	listName
 */
function openList(listName, save, openTitle, closeTitle) {
	var element = document.getElementById(listName);
	var status = 0;
	if (element.style.display == 'none') {
		// open list
		element.style.display = '';
		status = 1;
		var image = document.getElementById(listName + 'Image');
		if (image) {
			image.src = image.src.replace(/plus/, 'minus');
			if (closeTitle) image.title = closeTitle;
		}
	}
	else {
		// close list
		element.style.display = 'none';
		var image = document.getElementById(listName + 'Image');
		if (image) {
			image.src = image.src.replace(/minus/, 'plus');
			if (openTitle) image.title = openTitle;
		}
	}
	
	// save new status in session (use ajax)
	if (save) saveStatus(listName, status);
	
	return true;
}

/**
 * Saves the status of a hidden user option.
 */
function saveStatus(varname, value) {
	var ajaxRequest = new AjaxRequest();
	ajaxRequest.openPost('index.php?action=StatusSave'+SID_ARG_2ND, 'name='+encodeURIComponent(varname)+'&status='+encodeURIComponent(value));
}

function initList(listName, status) {
	if (!status) {
		var element = document.getElementById(listName);
		element.style.display = 'none';
		document.getElementById(listName + 'Image').src = document.getElementById(listName + 'Image').src.replace(/minus/, 'plus');
	}
}

/**
 * Shows a hidden content box.
 */
function showContent(content, link) {
	document.getElementById(link).style.display = 'none';
	document.getElementById(content).style.display = '';
	
	return;
}

/**
 * Fixes a browser bug with relative URLs ignoring the <base> tag.
 * Thanks to microsoft and opera ;)
 */
function fixURL(url) {
	if (IS_IE || IS_OPERA) {
		if (url.indexOf("/") == -1 && document.getElementsByTagName('base').length > 0) {
			return document.getElementsByTagName('base')[0].href + url;
		}
	}
	
	return url;
}

/**
* Wrapper for Editor function call to avoid js errors when no editor exists
*/
function WysiwygInsert(type, value, title, code) {
	if (typeof(tinyMCE) != 'undefined') {
		if (type == 'smiley') tinyMCE.insertSmiley(value, title, code);
		else if (type == 'attachment') tinyMCE.insertAttachment(value);
		else if (type == 'text') tinyMCE.insertText(value);
	}
	else {
		// TODO: insert plain text !? (to do this the function "insertCode" got to be in this default.js)
	}
}