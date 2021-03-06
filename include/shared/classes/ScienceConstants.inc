<?
include_once (dirname(__FILE__).'/Language.inc');
Language::get_instance()->load_language_file('ScienceConstants');
Language::get_instance()->load_language_file('ScienceConstantsBernard');

class ScienceConstants {  // Namespace!
	public static $VERY_IMPORTANT_SCS = array(
		4, 5, 12, 16, 22, 29, 31, 40, 41, 42, 1006, 3009
	);

	public static $ALL_SCIENCES = array(
		1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13, 14, 16, 17, 18, 22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 1001, 1002, 1004, 1006, 1007, 1009, 3001, 3002, 3007, 3008, 3009, 3010, 3011, 3012, 3013, 3014, 3016, 3017, 3019, 3020, 3021, 3022, 4001, 4002, 4003, 4004, 4005, 4006, 4007, 4008, 4009, 4010, 4011, 4012, 4013, 4014, 4015, 4016
	);

	public static $ALL_RACES = array(
		3001, 3002, 3007, 3008, 3009, 3011, 3014, 3016, 3021, 3022
	);

	public static $ALL_TOOLS = array(
		3010, 3012, 3013, 3017, 3019, 3020
	);

	public static $MAX_LEVELS = array(
		1 => 1, 2 => 1, 3 => 1, 4 => 1, 5 => 1, 7 => 1, 8 => 1, 9 => 1, 10 => 1, 12 => 1, 13 => 1, 14 => 1, 16 => 1, 17 => 1, 18 => 1, 22 => 1, 23 => 1, 24 => 1, 25 => 1, 26 => 1, 27 => 1, 29 => 1, 30 => 1, 31 => 1, 32 => 1, 33 => 1, 34 => 1, 35 => 1, 36 => 1, 39 => 1, 40 => 1, 41 => 1, 42 => 1, 43 => 1, 44 => 1, 45 => 1, 46 => 1, 47 => 1, 48 => 1, 49 => 1, 50 => 1, 51 => 1, 52 => 1, 53 => 1, 54 => 1, 55 => 1, 1001 => 1, 1002 => 1, 1004 => 1, 1006 => 1, 1007 => 1, 1009 => 1, 3001 => 1, 3002 => 1, 3007 => 1, 3008 => 1, 3009 => 1, 3010 => 1, 3011 => 1, 3012 => 1, 3013 => 1, 3014 => 1, 3016 => 1, 3017 => 1, 3019 => 1, 3020 => 1, 3021 => 1, 3022 => 1, 4001 => 1, 4002 => 1, 4003 => 1, 4004 => 1, 4005 => 1, 4006 => 1, 4007 => 1, 4008 => 1, 4009 => 1, 4010 => 1, 4011 => 1, 4012 => 1, 4013 => 1, 4014 => 1, 4015 => 1, 4016 => 1
	);

	public static $PICS = array(
		1 => 'pattern/spacer.gif',
		2 => 'scientist.gif',
		3 => 'pattern/spacer.gif',
		4 => 'pattern/spacer.gif',
		5 => 'pattern/spacer.gif',
		7 => 'pattern/spacer.gif',
		8 => 'pattern/spacer.gif',
		9 => 'pattern/spacer.gif',
		10 => 'pattern/spacer.gif',
		12 => 'pattern/spacer.gif',
		13 => 'pattern/spacer.gif',
		14 => 'pattern/spacer.gif',
		16 => 'pattern/spacer.gif',
		17 => 'pattern/spacer.gif',
		18 => 'pattern/spacer.gif',
		22 => 'spacer.gif',
		23 => 'pattern/spacer.gif',
		24 => 'pattern/spacer.gif',
		25 => 'pattern/spacer.gif',
		26 => 'pattern/spacer.gif',
		27 => 'pattern/spacer.gif',
		29 => 'pattern/spacer.gif',
		30 => 'pattern/spacer.gif',
		31 => 'pattern/spacer.gif',
		32 => 'pattern/spacer.gif',
		33 => 'pattern/spacer.gif',
		34 => 'pattern/spacer.gif',
		35 => 'pattern/spacer.gif',
		36 => 'pattern/spacer.gif',
		39 => 'spacer.gif',
		40 => 'pattern/spacer.gif',
		41 => 'pattern/spacer.gif',
		42 => 'pattern/spacer.gif',
		43 => 'pattern/spacer.gif',
		44 => 'pattern/spacer.gif',
		45 => 'pattern/spacer.gif',
		46 => 'pattern/spacer.gif',
		47 => 'pattern/spacer.gif',
		48 => 'pattern/spacer.gif',
		49 => 'pattern/spacer.gif',
		50 => 'pattern/spacer.gif',
		51 => 'pattern/spacer.gif',
		52 => 'pattern/spacer.gif',
		53 => 'pattern/spacer.gif',
		54 => 'pattern/spacer.gif',
		55 => 'pattern/spacer.gif',
		1001 => 'pattern/spacer.gif',
		1002 => 'pattern/spacer.gif',
		1004 => 'pattern/spacer.gif',
		1006 => 'pattern/spacer.gif',
		1007 => 'pattern/spacer.gif',
		1009 => 'pattern/spacer.gif',
		3001 => 'javelin.png',
		3002 => 'bow_stone.png',
		3007 => 'bow.png',
		3008 => 'javelin_stone.png',
		3009 => 'blade.png',
		3010 => 'pattern/spacer.gif',
		3011 => 'club.png',
		3012 => 'pattern/spacer.gif',
		3013 => 'pattern/spacer.gif',
		3014 => 'axe.png',
		3016 => 'axe_sharpened.png',
		3017 => 'pattern/spacer.gif',
		3019 => 'pattern/spacer.gif',
		3020 => 'pattern/spacer.gif',
		3021 => 'bow_sharpened.png',
		3022 => 'javelin_sharpened.png',
		4001 => 'camouflage.png',
		4002 => 'move_trees.png',
		4003 => 'stick_parry.png',
		4004 => 'train_abdominals.png',
		4005 => 'push_up.png',
		4006 => 'block.png',
		4007 => 'two_axes.png',
		4008 => 'bow_aiming.png',
		4009 => 'bow_direct_shot.png',
		4010 => 'bow_moving_target.png',
		4011 => 'atlatl.png',
		4012 => 'javelin_aiming.png',
		4013 => 'javelin_direct_shot.png',
		4014 => 'javelin_moving_target.png',
		4015 => 'herbs.png',
		4016 => 'rope_skipping.png'
	);

	public static $SC_NAME = array(
		1 => LNG_MORPHING,
		2 => LNG_SCIENTISTS,
		3 => LNG_TREES,
		4 => LNG_STONES,
		5 => LNG_PLANTS,
		7 => LNG_BIRCH,
		8 => LNG_LUMBER,
		9 => LNG_BARK,
		10 => LNG_BASALT,
		12 => LNG_FLINTSTONE,
		13 => LNG_PYRIT,
		14 => LNG_HEDGE,
		16 => LNG_BAST,
		17 => LNG_FLAX,
		18 => LNG_HEMP,
		22 => LNG_FIRE,
		23 => LNG_GELATINE,
		24 => LNG_RESIN,
		25 => LNG_BIRCH_PITCH,
		26 => LNG_CONTAINERS,
		27 => LNG_JOINT,
		29 => LNG_FIBRE,
		30 => LNG_CORD,
		31 => LNG_KNOT,
		32 => LNG_NETS,
		33 => LNG_SLING,
		34 => LNG_GLUE,
		35 => LNG_CATCH_PARTRIDGE,
		36 => LNG_SHARPEN,
		39 => LNG_REED,
		40 => LNG_SKINNING,
		41 => LNG_HUNTING,
		42 => LNG_WEAPON_REVOLUTION,
		43 => LNG_CHOP_DOWN_TREES,
		44 => LNG_CREATE_HEDGE,
		45 => LNG_COOK_MEAT,
		46 => LNG_QUAIL_EGGS,
		47 => LNG_CHICKEN_EGGS,
		48 => LNG_DUCK_EGGS,
		49 => LNG_GOOSE_EGGS,
		50 => LNG_CORNUS,
		51 => LNG_BOWSTRING,
		52 => LNG_BOW_PURE,
		53 => LNG_ARROWS,
		54 => LNG_BONES,
		55 => LNG_CHOP_DOWN_TREES2,
		1001 => LNG_REED_ROOF,
		1002 => LNG_HUT,
		1004 => LNG_LUMBERJACKS_HUT,
		1006 => LNG_ANIMAL_SKIN_TENT,
		1007 => LNG_FARM,
		1009 => LNG_SHED,
		3001 => LNG_JAVELIN,
		3002 => LNG_ADV_BOW,
		3007 => LNG_BOW,
		3008 => LNG_ADV_JAVELIN,
		3009 => LNG_BLADE,
		3010 => LNG_DRILL,
		3011 => LNG_CLUB,
		3012 => LNG_ADZE_BEATEN,
		3013 => LNG_ADZE_SHARPENED,
		3014 => LNG_AXE_BEATEN,
		3016 => LNG_AXE_SHARPENED,
		3017 => LNG_DIGWOOD,
		3019 => LNG_WHEEL,
		3020 => LNG_WALZENBEIL,
		3021 => LNG_ADV_BOW_SHARPENED,
		3022 => LNG_ADV_JAVELIN_SHARPENED,
		4001 => LNG_CAMOUFLAGE,
		4002 => LNG_MOVE_TREES,
		4003 => LNG_STICK_PARRY,
		4004 => LNG_TRAIN_ABDOMINALS,
		4005 => LNG_PUSH_UP,
		4006 => LNG_AXE_BLOCK,
		4007 => LNG_TWO_AXES,
		4008 => LNG_BOW_AIMING,
		4009 => LNG_BOW_DIRECT_SHOT,
		4010 => LNG_BOW_MOVING_TARGETS,
		4011 => LNG_ATLATL,
		4012 => LNG_JAVELIN_AIM,
		4013 => LNG_JAVELIN_DIRECT_THROW,
		4014 => LNG_JAVELIN_MOVING_TARGETS,
		4015 => LNG_HERBS,
		4016 => LNG_ROPE_SKIPPING
	);
	public static $SC_DESC = array(
		1 => LNG_MORPHING_DESC,
		2 => LNG_SCIENTISTS_DESC,
		3 => LNG_TREES_DESC,
		4 => LNG_STONES_DESC,
		5 => LNG_PLANTS_DESC,
		7 => LNG_BIRCH_DESC,
		8 => LNG_LUMBER_DESC,
		9 => LNG_BARK_DESC,
		10 => LNG_BASALT_DESC,
		12 => LNG_FLINTSTONE_DESC,
		13 => LNG_PYRIT_DESC,
		14 => LNG_HEDGE_DESC,
		16 => LNG_BAST_DESC,
		17 => LNG_FLAX_DESC,
		18 => LNG_HEMP_DESC,
		22 => LNG_FIRE_DESC,
		23 => LNG_GELATINE_DESC,
		24 => LNG_RESIN_DESC,
		25 => LNG_BIRCH_PITCH_DESC,
		26 => LNG_CONTAINERS_DESC,
		27 => LNG_JOINT_DESC,
		29 => LNG_FIBRE_DESC,
		30 => LNG_CORD_DESC,
		31 => LNG_KNOT_DESC,
		32 => LNG_NETS_DESC,
		33 => LNG_SLING_DESC,
		34 => LNG_GLUE_DESC,
		35 => LNG_CATCH_PARTRIDGE_DESC,
		36 => LNG_SHARPEN_DESC,
		39 => LNG_REED_DESC,
		40 => LNG_SKINNING_DESC,
		41 => LNG_HUNTING_DESC,
		42 => LNG_WEAPON_REVOLUTION_DESC,
		43 => LNG_CHOP_DOWN_TREES_DESC,
		44 => LNG_CREATE_HEDGE_DESC,
		45 => LNG_COOK_MEAT_DESC,
		46 => LNG_QUAIL_EGGS_DESC,
		47 => LNG_CHICKEN_EGGS_DESC,
		48 => LNG_DUCK_EGGS_DESC,
		49 => LNG_GOOSE_EGGS_DESC,
		50 => LNG_CORNUS_DESC,
		51 => LNG_BOWSTRING_DESC,
		52 => LNG_BOW_PURE_DESC,
		53 => LNG_ARROWS_DESC,
		54 => LNG_BONES_DESC,
		55 => LNG_CHOP_DOWN_TREES2_DESC,
		1001 => LNG_REED_ROOF_DESC,
		1002 => LNG_HUT_DESC,
		1004 => LNG_LUMBERJACKS_HUT_DESC,
		1006 => LNG_ANIMAL_SKIN_TENT_DESC,
		1007 => LNG_FARM_DESC,
		1009 => LNG_SHED_DESC,
		3001 => LNG_JAVELIN_DESC,
		3002 => LNG_ADV_BOW_DESC,
		3007 => LNG_BOW_DESC,
		3008 => LNG_ADV_JAVELIN_DESC,
		3009 => LNG_BLADE_DESC,
		3010 => LNG_DRILL_DESC,
		3011 => LNG_CLUB_DESC,
		3012 => LNG_ADZE_BEATEN_DESC,
		3013 => LNG_ADZE_SHARPENED_DESC,
		3014 => LNG_AXE_BEATEN_DESC,
		3016 => LNG_AXE_SHARPENED_DESC,
		3017 => LNG_DIGWOOD_DESC,
		3019 => LNG_WHEEL_DESC,
		3020 => LNG_WALZENBEIL_DESC,
		3021 => LNG_ADV_BOW_SHARPENED_DESC,
		3022 => LNG_ADV_JAVELIN_SHARPENED_DESC,
		4001 => LNG_CAMOUFLAGE_DESC,
		4002 => LNG_MOVE_TREES_DESC,
		4003 => LNG_STICK_PARRY_DESC,
		4004 => LNG_TRAIN_ABDOMINALS_DESC,
		4005 => LNG_PUSH_UP_DESC,
		4006 => LNG_AXE_BLOCK_DESC,
		4007 => LNG_TWO_AXES_DESC,
		4008 => LNG_BOW_AIMING_DESC,
		4009 => LNG_BOW_DIRECT_SHOT_DESC,
		4010 => LNG_BOW_MOVING_TARGETS_DESC,
		4011 => LNG_ATLATL_DESC,
		4012 => LNG_JAVELIN_AIM_DESC,
		4013 => LNG_JAVELIN_DIRECT_THROW_DESC,
		4014 => LNG_JAVELIN_MOVING_TARGETS_DESC,
		4015 => LNG_HERBS_DESC,
		4016 => LNG_ROPE_SKIPPING_DESC
	);
	public static $SC_DESC_BERNARD = array(
		1 => LNG_MORPHING_BERNARD,
		2 => LNG_SCIENTISTS_BERNARD,
		3 => LNG_TREES_BERNARD,
		4 => LNG_STONES_BERNARD,
		5 => LNG_PLANTS_BERNARD,
		7 => LNG_BIRCH_BERNARD,
		8 => LNG_LUMBER_BERNARD,
		9 => LNG_BARK_BERNARD,
		10 => LNG_BASALT_BERNARD,
		12 => LNG_FLINTSTONE_BERNARD,
		13 => LNG_PYRIT_BERNARD,
		14 => LNG_HEDGE_BERNARD,
		16 => LNG_BAST_BERNARD,
		17 => LNG_FLAX_BERNARD,
		18 => LNG_HEMP_BERNARD,
		22 => LNG_FIRE_BERNARD,
		23 => LNG_GELATINE_BERNARD,
		24 => LNG_RESIN_BERNARD,
		25 => LNG_BIRCH_PITCH_BERNARD,
		26 => LNG_CONTAINERS_BERNARD,
		27 => LNG_JOINT_BERNARD,
		29 => LNG_FIBRE_BERNARD,
		30 => LNG_CORD_BERNARD,
		31 => LNG_KNOT_BERNARD,
		32 => LNG_NETS_BERNARD,
		33 => LNG_SLING_BERNARD,
		34 => LNG_GLUE_BERNARD,
		35 => LNG_CATCH_PARTRIDGE_BERNARD,
		36 => LNG_SHARPEN_BERNARD,
		39 => LNG_REED_BERNARD,
		40 => LNG_SKINNING_BERNARD,
		41 => LNG_HUNTING_BERNARD,
		42 => LNG_WEAPON_REVOLUTION_BERNARD,
		43 => LNG_CHOP_DOWN_TREES_BERNARD,
		44 => LNG_CREATE_HEDGE_BERNARD,
		45 => LNG_COOK_MEAT_BERNARD,
		46 => LNG_QUAIL_EGGS_BERNARD,
		47 => LNG_CHICKEN_EGGS_BERNARD,
		48 => LNG_DUCK_EGGS_BERNARD,
		49 => LNG_GOOSE_EGGS_BERNARD,
		50 => LNG_CORNUS_BERNARD,
		51 => LNG_BOWSTRING_BERNARD,
		52 => LNG_BOW_PURE_BERNARD,
		53 => LNG_ARROWS_BERNARD,
		54 => LNG_BONES_BERNARD,
		55 => LNG_CHOP_DOWN_TREES2_BERNARD,
		1001 => LNG_REED_ROOF_BERNARD,
		1002 => LNG_HUT_BERNARD,
		1004 => LNG_LUMBERJACKS_HUT_BERNARD,
		1006 => LNG_ANIMAL_SKIN_TENT_BERNARD,
		1007 => LNG_FARM_BERNARD,
		1009 => LNG_SHED_BERNARD,
		3001 => LNG_JAVELIN_BERNARD,
		3002 => LNG_ADV_BOW_BERNARD,
		3007 => LNG_BOW_BERNARD,
		3008 => LNG_ADV_JAVELIN_BERNARD,
		3009 => LNG_BLADE_BERNARD,
		3010 => LNG_DRILL_BERNARD,
		3011 => LNG_CLUB_BERNARD,
		3012 => LNG_ADZE_BEATEN_BERNARD,
		3013 => LNG_ADZE_SHARPENED_BERNARD,
		3014 => LNG_AXE_BEATEN_BERNARD,
		3016 => LNG_AXE_SHARPENED_BERNARD,
		3017 => LNG_DIGWOOD_BERNARD,
		3019 => LNG_WHEEL_BERNARD,
		3020 => LNG_WALZENBEIL_BERNARD,
		3021 => LNG_ADV_BOW_SHARPENED_BERNARD,
		3022 => LNG_ADV_JAVELIN_SHARPENED_BERNARD,
		4001 => LNG_CAMOUFLAGE_BERNARD,
		4002 => LNG_MOVE_TREES_BERNARD,
		4003 => LNG_STICK_PARRY_BERNARD,
		4004 => LNG_TRAIN_ABDOMINALS_BERNARD,
		4005 => LNG_PUSH_UP_BERNARD,
		4006 => LNG_AXE_BLOCK_BERNARD,
		4007 => LNG_TWO_AXES_BERNARD,
		4008 => LNG_BOW_AIMING_BERNARD,
		4009 => LNG_BOW_DIRECT_SHOT_BERNARD,
		4010 => LNG_BOW_MOVING_TARGETS_BERNARD,
		4011 => LNG_ATLATL_BERNARD,
		4012 => LNG_JAVELIN_AIM_BERNARD,
		4013 => LNG_JAVELIN_DIRECT_THROW_BERNARD,
		4014 => LNG_JAVELIN_MOVING_TARGETS_BERNARD,
		4015 => LNG_HERBS_BERNARD,
		4016 => LNG_ROPE_SKIPPING_BERNARD
	);
	public static $SC_DESC_KNOWN = array(
		1 => LNG_MORPHING_KNOWN,
		2 => LNG_SCIENTISTS_KNOWN,
		3 => LNG_TREES_KNOWN,
		4 => LNG_STONES_KNOWN,
		5 => LNG_PLANTS_KNOWN,
		7 => LNG_BIRCH_KNOWN,
		8 => LNG_LUMBER_KNOWN,
		9 => LNG_BARK_KNOWN,
		10 => LNG_BASALT_KNOWN,
		12 => LNG_FLINTSTONE_KNOWN,
		13 => LNG_PYRIT_KNOWN,
		14 => LNG_HEDGE_KNOWN,
		16 => LNG_BAST_KNOWN,
		17 => LNG_FLAX_KNOWN,
		18 => LNG_HEMP_KNOWN,
		22 => LNG_FIRE_KNOWN,
		23 => LNG_GELATINE_KNOWN,
		24 => LNG_RESIN_KNOWN,
		25 => LNG_BIRCH_PITCH_KNOWN,
		26 => LNG_CONTAINERS_KNOWN,
		27 => LNG_JOINT_KNOWN,
		29 => LNG_FIBRE_KNOWN,
		30 => LNG_CORD_KNOWN,
		31 => LNG_KNOT_KNOWN,
		32 => LNG_NETS_KNOWN,
		33 => LNG_SLING_KNOWN,
		34 => LNG_GLUE_KNOWN,
		35 => LNG_CATCH_PARTRIDGE_KNOWN,
		36 => LNG_SHARPEN_KNOWN,
		39 => LNG_REED_KNOWN,
		40 => LNG_SKINNING_KNOWN,
		41 => LNG_HUNTING_KNOWN,
		42 => LNG_WEAPON_REVOLUTION_KNOWN,
		43 => LNG_CHOP_DOWN_TREES_KNOWN,
		44 => LNG_CREATE_HEDGE_KNOWN,
		45 => LNG_COOK_MEAT_KNOWN,
		46 => LNG_QUAIL_EGGS_KNOWN,
		47 => LNG_CHICKEN_EGGS_KNOWN,
		48 => LNG_DUCK_EGGS_KNOWN,
		49 => LNG_GOOSE_EGGS_KNOWN,
		50 => LNG_CORNUS_KNOWN,
		51 => LNG_BOWSTRING_KNOWN,
		52 => LNG_BOW_PURE_KNOWN,
		53 => LNG_ARROWS_KNOWN,
		54 => LNG_BONES_KNOWN,
		55 => LNG_CHOP_DOWN_TREES2_KNOWN,
		1001 => LNG_REED_ROOF_KNOWN,
		1002 => LNG_HUT_KNOWN,
		1004 => LNG_LUMBERJACKS_HUT_KNOWN,
		1006 => LNG_ANIMAL_SKIN_TENT_KNOWN,
		1007 => LNG_FARM_KNOWN,
		1009 => LNG_SHED_KNOWN,
		3001 => LNG_JAVELIN_KNOWN,
		3002 => LNG_ADV_BOW_KNOWN,
		3007 => LNG_BOW_KNOWN,
		3008 => LNG_ADV_JAVELIN_KNOWN,
		3009 => LNG_BLADE_KNOWN,
		3010 => LNG_DRILL_KNOWN,
		3011 => LNG_CLUB_KNOWN,
		3012 => LNG_ADZE_BEATEN_KNOWN,
		3013 => LNG_ADZE_SHARPENED_KNOWN,
		3014 => LNG_AXE_BEATEN_KNOWN,
		3016 => LNG_AXE_SHARPENED_KNOWN,
		3017 => LNG_DIGWOOD_KNOWN,
		3019 => LNG_WHEEL_KNOWN,
		3020 => LNG_WALZENBEIL_KNOWN,
		3021 => LNG_ADV_BOW_SHARPENED_KNOWN,
		3022 => LNG_ADV_JAVELIN_SHARPENED_KNOWN,
		4001 => LNG_CAMOUFLAGE_KNOWN,
		4002 => LNG_MOVE_TREES_KNOWN,
		4003 => LNG_STICK_PARRY_KNOWN,
		4004 => LNG_TRAIN_ABDOMINALS_KNOWN,
		4005 => LNG_PUSH_UP_KNOWN,
		4006 => LNG_AXE_BLOCK_KNOWN,
		4007 => LNG_TWO_AXES_KNOWN,
		4008 => LNG_BOW_AIMING_KNOWN,
		4009 => LNG_BOW_DIRECT_SHOT_KNOWN,
		4010 => LNG_BOW_MOVING_TARGETS_KNOWN,
		4011 => LNG_ATLATL_KNOWN,
		4012 => LNG_JAVELIN_AIM_KNOWN,
		4013 => LNG_JAVELIN_DIRECT_THROW_KNOWN,
		4014 => LNG_JAVELIN_MOVING_TARGETS_KNOWN,
		4015 => LNG_HERBS_KNOWN,
		4016 => LNG_ROPE_SKIPPING_KNOWN
	);
	private function __construct(){} // all static!
}
?>
