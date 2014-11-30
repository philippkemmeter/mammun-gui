/**
 * @author	Marcel Werk
 * @copyright	2001-2007 WoltLab GmbH
 * @license	GNU Lesser General Public License <http://opensource.org/licenses/lgpl-license.php>
 */
function ImageResizer() {
	this.maxWidth = 530;
	
	this.resize = function() {
		for (var i = 0; i < document.images.length ;i++){
			if (document.images[i].className == 'resizeImage') {
				var imageWidth = document.images[i].width;
				var imageHeight = document.images[i].height;
				
				if (imageWidth > this.maxWidth) {
					document.images[i].width = this.maxWidth;
					document.images[i].height = Math.round(imageHeight * (this.maxWidth / imageWidth));
					
					if (!this.isLinked(document.images[i])) {
						var popupLink = document.createElement("a");
						popupLink.className = 'externalURL';
						popupLink.setAttribute('href', document.images[i].src);
						popupLink.appendChild(document.images[i].cloneNode(true));
						
						document.images[i].parentNode.replaceChild(popupLink, document.images[i]);
					}
				}
			}
		}
	}
	
	this.isLinked = function(node) {
		do {
			node = node.parentNode;
			if (node == undefined) break;
			if (node.nodeName == 'A') return true;
		}
		while (node.nodeName != 'TD' && node.nodeName != 'P' && node.nodeName != 'DIV' && node.nodeName != 'BODY');
			
		return false;
	}
	
	this.resize();
}

onloadEvents.push(function() { new ImageResizer(); });
