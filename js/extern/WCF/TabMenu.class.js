/**
 * @author	Marcel Werk
 * @copyright	2001-2007 WoltLab GmbH
 * @license	GNU Lesser General Public License <http://opensource.org/licenses/lgpl-license.php>
 */
function TabMenu() {
	this.activeTabMenuItem = '';
	this.activeSubTabMenuItem = '';
	this.activeSubTabMenuItems = new Object();

	this.showSubTabMenu = function(tabMenuItem, subTabMenuItem) {
		if (this.activeTabMenuItem == tabMenuItem) return;
				
		if (this.activeTabMenuItem) {
			this.hideSubTabMenu(this.activeTabMenuItem);
			this.setActiveTabMenuItem('');
		}
		
		// enable menu item
		var tabMenuItemLI = document.getElementById(tabMenuItem);
		if (tabMenuItemLI) tabMenuItemLI.className = 'activeTabMenu';
		
		// show sub categories
		var tabMenuItemUL = document.getElementById(tabMenuItem + '-categories');
		if (tabMenuItemUL) {
			tabMenuItemUL.className = '';
			
			if (!subTabMenuItem && this.activeSubTabMenuItems[tabMenuItem]) subTabMenuItem = this.activeSubTabMenuItems[tabMenuItem];
			if (!subTabMenuItem) {
				// get first sub category
				for (var i = 0; i < tabMenuItemUL.childNodes.length; i++) {
					if (tabMenuItemUL.childNodes[i].nodeName.toLowerCase() == 'li') {
						subTabMenuItem = tabMenuItemUL.childNodes[i].id;
						break;
					}
				}
			}
				
			this.showTabMenuContent(subTabMenuItem);
		}
		else {
			if (this.activeSubTabMenuItem != '') {
				this.hideTabMenuContent(this.activeSubTabMenuItem);
				this.setActiveSubTabMenuItem('');
			}
			document.getElementById(tabMenuItem + '-content').className = document.getElementById(tabMenuItem + '-content').className.replace(/ hidden/, '');
		}
		
		this.setActiveTabMenuItem(tabMenuItem);
	}
	
	this.hideSubTabMenu = function(tabMenuItem) {
		// disable menu item
		var activeTabMenuItemLI = document.getElementById(tabMenuItem);
		if (activeTabMenuItemLI) activeTabMenuItemLI.className = '';
		
		// hide sub categories
		var activeTabMenuItemUL = document.getElementById(tabMenuItem + '-categories');
		if (activeTabMenuItemUL) activeTabMenuItemUL.className = 'hidden';
		else document.getElementById(tabMenuItem + '-content').className += ' hidden';
	}
	
	this.hideTabMenuContent = function (subTabMenuItem) {
		document.getElementById(subTabMenuItem).className = '';
		document.getElementById(subTabMenuItem + '-content').className += ' hidden';
	}
	
	this.showTabMenuContent = function(subTabMenuItem) {
		if (this.activeSubTabMenuItem == subTabMenuItem) return;
		
		if (this.activeSubTabMenuItem) {
			this.hideTabMenuContent(this.activeSubTabMenuItem);
			this.setActiveSubTabMenuItem('');
		}
		
		document.getElementById(subTabMenuItem).className = 'activeSubTabMenu';
		document.getElementById(subTabMenuItem + '-content').className = document.getElementById(subTabMenuItem + '-content').className.replace(/ hidden/, '');
		this.setActiveSubTabMenuItem(subTabMenuItem);
	}
	
	this.setActiveTabMenuItem = function(activeTabMenuItem) {
		this.activeTabMenuItem = activeTabMenuItem;
		var hidden = document.getElementById('activeTabMenuItem');
		if (hidden) {
			hidden.value = activeTabMenuItem;
		}
	}
	
	this.setActiveSubTabMenuItem = function(activeSubTabMenuItem) {
		this.activeSubTabMenuItems[this.activeTabMenuItem] = activeSubTabMenuItem;
		this.activeSubTabMenuItem = activeSubTabMenuItem;
		var hidden = document.getElementById('activeSubTabMenuItem');
		if (hidden) {
			hidden.value = activeSubTabMenuItem;
		}
	}
}