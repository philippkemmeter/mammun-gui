/**
 * @author	Marcel Werk
 * @copyright	2001-2007 WoltLab GmbH
 * @license	GNU Lesser General Public License <http://opensource.org/licenses/lgpl-license.php>
 */
function Poll(pollID, choiceCount, pollOptionIDs) {
	this.pollID = pollID;
	this.choiceCount = choiceCount;
	this.pollOptionIDs = pollOptionIDs;
	this.checkboxesDisabled = false;
	
	this.init = function() {
		// add event listener
		for (var i = 0; i < this.pollOptionIDs.length; i++) {
			var checkbox = document.getElementById('pollOption'+this.pollOptionIDs[i]);
			checkbox.poll = this;
			checkbox.onclick = function() { this.poll.checkboxClicked(); }
		}
		
		this.checkboxClicked();
	}
	
	this.checkboxClicked = function() {
		// count checked checkboxes
		var count = 0;
		for (var i = 0; i < this.pollOptionIDs.length; i++) {
			var checkbox = document.getElementById('pollOption'+this.pollOptionIDs[i]);
			if (checkbox.checked) count++;
		}
		
		if (count >= this.choiceCount) {
			if (this.checkboxesDisabled) return;
			
			for (var i = 0; i < this.pollOptionIDs.length; i++) {
				var checkbox = document.getElementById('pollOption'+this.pollOptionIDs[i]);
				if (!checkbox.checked) checkbox.disabled = true;	
			}
			
			this.checkboxesDisabled = true;
		}
		else if (this.checkboxesDisabled) {
			for (var i = 0; i < this.pollOptionIDs.length; i++) {
				var checkbox = document.getElementById('pollOption'+this.pollOptionIDs[i]);
				checkbox.disabled = false;	
			}
			
			this.checkboxesDisabled = false;
		}
	}
	
	this.init();
}