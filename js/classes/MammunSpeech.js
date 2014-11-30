/**
 * Diese Klasse lässt Mammuns reden :)
 * 
 * @auther Phil
 */

MammunSpeech = new Object();	// rein statisch

MammunSpeech.explt = {
	'my': {
		'fight' : {
			'lowhp': [''],
			'club': [''],
			'javelin': [''],
			'blade': [''],
			'bow': ['']
		},
		'nofight': {
			'lowhp': [''],
			'naked': [''],
			'club': [''],
			'javelin': [''],
			'blade': [''],
			'bow': ['']
		}
	},
	'theirs': {
		'fight' : {
			'club': [''],
			'javelin': [''],
			'blade': [''],
			'bow': ['']
		},
		'nofight' : {
			'naked': [''],
			'club': [''],
			'javelin': [''],
			'blade': [''],
			'bow': ['']
		}
	}
}


MammunSpeech.get_speech = function(unit_info, fight, my_mammun) {
	var tmp = unit_info.pic_url.split('/');
	tmp = tmp[tmp.length-1].split('_');
	var weapon = tmp[0];
	switch (weapon) {
		case 'axe':
		case 'axe_sharpened':
			weapon = 'club';
			break;
		case 'bow_stone':
		case 'bow_sharpened':
			weapon = 'bow';
			break;
		case 'javelin_stone':
		case 'javelin_sharpened':
			weapon = 'javelin';
			break;
	}
	if (my_mammun) {
		if (fight) {
			if ((unit_info.amount/1000*unit_info.hp) < 20)
				return MammunSpeech.explt['my']['fight']['lowhp'].random();
			else
				return MammunSpeech.explt['my']['fight'][weapon].random();
		}
		else {
			if ((unit_info.amount/1000*unit_info.hp) < 30)
				return MammunSpeech.explt['my']['nofight']['lowhp'].random();
			else if (unit_info.naked)
				return MammunSpeech.explt['my']['nofight']['naked'].random();
			else
				return MammunSpeech.explt['my']['nofight'][weapon].random();
		}
	}
	else {
		if (fight) {
			return MammunSpeech.explt['theirs']['fight'][weapon].random();
		}
		else {
			if (unit_info.naked)
				return MammunSpeech.explt['theirs']['nofight']['naked'].random();
			else
				return MammunSpeech.explt['theirs']['nofight'][weapon].random();
		}
	}
}