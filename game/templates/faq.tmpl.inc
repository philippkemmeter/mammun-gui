<script>
<?
	/**
	 * WARNING: The implementation of this window is very much static. Any
	 * changes to height or width of any of the elements could cause erratic
	 * behavior. The pixel height and width of the different elements is based
	 * almost entirely on the pictures placed within them.
	 */
?>
if (PureFW.Officer.get_current_active_officer())
	PureFW.Officer.get_current_active_officer().hide();

FAQ = new Object();
FAQ.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
FAQ.current_text_containers = new Array();

FAQ.general_questions = new Array(
		'<?=LNG_IDEA_GAME?>',
		'<?=LNG_WHICH_DATA_MUST_I_PROVIDE?>',
		'<?=LNG_FORWARDING_DATA?>',
		'<?=LNG_HOW_EARNS_GAUSUS_MONEY?>'<?/*,
		'<?=LNG_ANSWER_NOT_FOUND?>'*/?>);
FAQ.general_texts = new Array(
		'<?=LNG_IDEA_GAME_TEXT?>',
		'<?=	str_replace(
					array('[LINK DATENSCHUTZ]', '[/LINK]'),
					array(
						'<a style="color:white;" href="javascript: UIButtons.open_new_window('
							. '\\\'/datenschutz.html\\\','
							. 'map_ui__fixed_window.get_z_index()+1, '
							. '141, 100,'
							. '717, 484);">',
						'</a>'
					),
					LNG_WHICH_DATA_MUST_I_PROVIDE_TEXT
				)?>',
		'<?=LNG_FORWARDING_DATA_TEXT?>',
		'<?=LNG_HOW_EARNS_GAUSUS_MONEY_TEXT?>'<?/*,
		'<?=LNG_ANSWER_NOT_FOUND_TEXT?>'*/?>);
FAQ.general_bullets = new Array();

FAQ.game_before_questions = new Array(
		'<?=LNG_START_SITUATION?>',
		'<?=LNG_GOAL_OF_GAME?>',
		'<?=LNG_DIFFERENCES_BETWEEN_ISLANDS?>',
		'<?=LNG_IMPORTANCE_ISLAND_SELECTION?>',
		'<?=LNG_HOW_TO_SCORE?>');
FAQ.game_before_texts = new Array(
		'<?=LNG_START_SITUATION_TEXT?>',
		'<?=LNG_GOAL_OF_GAME_TEXT?>',
		'<?=LNG_DIFFERENCES_BETWEEN_ISLANDS_TEXT?>',
		'<?=LNG_IMPORTANCE_ISLAND_SELECTION_TEXT?>',
		'<?=LNG_HOW_TO_SCORE_TEXT?>');
FAQ.game_before_bullets = new Array();

FAQ.game_char_questions = new Array(
		'<?=LNG_DEFINITION_BERNARD?>',
		'<?=LNG_MAXIMAL_NUMBER_PEOPLE?>',
		'<?=LNG_DIFFERENCE_EXPLORER_THINKER?>',
		<?/*'<?=LNG_ONCE_THINKER_ALWAYS_THINKER?>',
		'<?=LNG_ONCE_EXPLORER_ALWAYS_EXPLORER?>',*/?>
		'<?=LNG_DEFINITION_FIGHT?>',
		'<?=LNG_WHEN_FIGHT?>',
		'<?=LNG_VARIABLES_RESULT_FIGHT?>',
		'<?=LNG_WHAT_IF_HOSTILE_WINS?>',
		'<?=LNG_DEFINITION_SECTOR?>',
		'<?=LNG_DEFINITION_HOMESECTOR?>');
FAQ.game_char_texts = new Array(
		'<?=LNG_DEFINITION_BERNARD_TEXT?>',
		'<?=LNG_MAXIMAL_NUMBER_PEOPLE_TEXT?>',
		'<?=LNG_DIFFERENCE_EXPLORER_THINKER_TEXT?>',
		<?/*'<?=LNG_ONCE_THINKER_ALWAYS_THINKER_TEXT?>',
		'<?=LNG_ONCE_EXPLORER_ALWAYS_EXPLORER_TEXT?>',*/?>
		'<?=LNG_DEFINITION_FIGHT_TEXT?>',
		'<?=LNG_WHEN_FIGHT_TEXT?>',
		'<?=LNG_VARIABLES_RESULT_FIGHT_TEXT?>',
		'<?=LNG_WHAT_IF_HOSTILE_WINS_TEXT?>',
		'<?=LNG_DEFINITION_SECTOR_TEXT?>',
		'<?=LNG_DEFINITION_HOMESECTOR_TEXT?>');
FAQ.game_char_bullets = new Array();

FAQ.task_research_questions = new Array(
		'<?=LNG_DEFINITION_THINKER?>',
		'<?=LNG_NUMBER_THINKERS_SET?>',
		'<?=LNG_DEFINITION_INVENTION?>',
		'<?=LNG_WHY_INVENT?>',
		'<?=LNG_HOW_TO_INVENT?>',
		'<?=LNG_WHEN_TRAIN_THINKERS?>',
		'<?=LNG_HOW_TO_TRAIN_THINKERS?>');
FAQ.task_research_texts = new Array(
		'<?=LNG_DEFINITION_THINKER_TEXT?>',
		'<?=LNG_NUMBER_THINKERS_SET_TEXT?>',
		'<?=LNG_DEFINITION_INVENTION_TEXT?>',
		'<?=LNG_WHY_INVENT_TEXT?>',
		'<?=LNG_HOW_TO_INVENT_TEXT?>',
		'<?=LNG_WHEN_TRAIN_THINKERS_TEXT?>',
		'<?=LNG_HOW_TO_TRAIN_THINKERS_TEXT?>');
FAQ.task_research_bullets = new Array();

FAQ.task_explore_questions = new Array(
		'<?=LNG_DEFINITION_EXPLORER?>',
		'<?=LNG_HOW_TO_MOVE_EXPLORER?>',
		'<?=LNG_WHERE_CAN_EXPLORER_GO?>',
		'<?=LNG_WHAT_IF_SECTOR_OCCUPIED?>',
		'<?=LNG_WHAT_IF_SECTOR_HOST?>',
		'<?=LNG_NUMBER_EXPLORER_SET?>',
		'<?=LNG_WHY_TRAIN_EXPLORER?>',
		'<?=LNG_CONNECTION_THINKERS_TRAINING?>',
		'<?=LNG_WHEN_TRAIN_EXPLORERS?>',
		'<?=LNG_HOW_TO_TRAIN_EXPLORERS?>',
		'<?=LNG_WHEN_IS_SECTOR_MINE?>');
FAQ.task_explore_texts = new Array(
		'<?=LNG_DEFINITION_EXPLORER_TEXT?>',
		'<?=LNG_HOW_TO_MOVE_EXPLORER_TEXT?>',
		'<?=LNG_WHERE_CAN_EXPLORER_GO_TEXT?>',
		'<?=LNG_WHAT_IF_SECTOR_OCCUPIED_TEXT?>',
		'<?=LNG_WHAT_IF_SECTOR_HOST_TEXT?>',
		'<?=LNG_NUMBER_EXPLORER_SET_TEXT?>',
		'<?=LNG_WHY_TRAIN_EXPLORER_TEXT?>',
		'<?=LNG_CONNECTION_THINKERS_TRAINING_TEXT?>',
		'<?=LNG_WHEN_TRAIN_EXPLORERS_TEXT?>',
		'<?=LNG_HOW_TO_TRAIN_EXPLORERS_TEXT?>',
		'<?=LNG_WHEN_IS_SECTOR_MINE_TEXT?>');
FAQ.task_explore_bullets = new Array();

FAQ.task_build_questions = new Array(
		'<?=LNG_WHY_CONSTRUCT_BUILDINGS?>',
		'<?=LNG_WHEN_STARTS_CONSTRUCTION?>',
		'<?=LNG_HOW_TO_CONSTRUCT?>',
		'<?=LNG_WHERE_TO_CONSTRUCT?>',
		'<?=LNG_HOW_MANY_BUILDINGS_TO_CONSTRUCT?>',
		'<?=LNG_CAN_I_TEAR_DOWN_BUILDINGS?>');
FAQ.task_build_texts = new Array(
		'<?=LNG_WHY_CONSTRUCT_BUILDINGS_TEXT?>',
		'<?=LNG_WHEN_STARTS_CONSTRUCTION_TEXT?>',
		'<?=LNG_HOW_TO_CONSTRUCT_TEXT?>',
		'<?=LNG_WHERE_TO_CONSTRUCT_TEXT?>',
		'<?=LNG_HOW_MANY_BUILDINGS_TO_CONSTRUCT_TEXT?>',
		'<?=LNG_CAN_I_TEAR_DOWN_BUILDINGS_TEXT?>');
FAQ.task_build_bullets = new Array();

FAQ.task_buy_questions = new Array(
		'<?=LNG_DEFINITION_TREASURE_BOX?>',
		'<?=LNG_WHERE_TO_BUY_TREASURES?>',
		'<?=LNG_HOW_TO_PURCHASE_TREASURES?>'<?/*,
		'<?=LNG_HOW_TO_SELL_TREASURES?>'*/?>);
FAQ.task_buy_texts = new Array(
		'<?=LNG_DEFINITION_TREASURE_BOX_TEXT?>',
		'<?=LNG_WHERE_TO_BUY_TREASURES_TEXT?>',
		'<?=LNG_HOW_TO_PURCHASE_TREASURES_TEXT?>'<?/*,
		'<?=LNG_HOW_TO_SELL_TREASURES_TEXT?>'*/?>);
FAQ.task_buy_bullets = new Array();

FAQ.task_treasure_questions = new Array(
		'<?=LNG_WHAT_TO_DO_WITH_TREASURES?>',
		'<?=LNG_APPLY_TREASURES?>',
		'<?=LNG_WHO_DESIGNS_TREASURES?>'<?/*,
		'<?=LNG_CAN_I_DESIGN_TREASURES?>'*/?>);
FAQ.task_treasure_texts = new Array(
		'<?=LNG_WHAT_TO_DO_WITH_TREASURES_TEXT?>',
		'<?=LNG_APPLY_TREASURES_TEXT?>',
		'<?=LNG_WHO_DESIGNS_TREASURES_TEXT?>'<?/*,
		'<?=LNG_CAN_I_DESIGN_TREASURES_TEXT?>'*/?>);
FAQ.task_treasure_bullets = new Array();

FAQ.klunker_questions = new Array(
		'<?=LNG_WHY_PURCHASE_KKR?>',
		'<?=LNG_ADVANTAGE_OF_KKR?>',
		'<?=LNG_HOW_TO_PURCHASE_KKR?>',
		'<?=LNG_PURCHASED_KKR_BUT_NOT_ON_ACCOUNT?>',
		<?/*'<?=LNG_CAN_I_CALL_OFF_PURCHASE_OF_KKR?>',*/?>
		'<?=LNG_CAN_I_CALL_OFF_PURCHASE_OF_ITEMS?>',
		'<?=LNG_CAN_I_CALL_OFF_TRADE_OF_ITEMS_SELLER?>',
		'<?=LNG_CAN_I_CALL_OFF_TRADE_OF_ITEMS_BUYER?>',
		'<?=LNG_GIVE_AWAY_KKR?>',
		'<?=LNG_HOW_CAN_I_PAY?>'<?/*,
		'<?=LNG_HOW_DOES_MOBILE_PAYMENT_WORK?>',
		'<?=LNG_MOBILE_PAYMENT_SPAM?>',
		'<?=LNG_WHICH_MOBILE_PROVIDERS_SUPPORTED?>'*/?>);
FAQ.klunker_texts = new Array(
		'<?=LNG_WHY_PURCHASE_KKR_TEXT?>',
		'<?=LNG_ADVANTAGE_OF_KKR_TEXT?>',
		'<?=LNG_HOW_TO_PURCHASE_KKR_TEXT?>',
		'<?=LNG_PURCHASED_KKR_BUT_NOT_ON_ACCOUNT_TEXT?>',
		<?/*'<?=LNG_CAN_I_CALL_OFF_PURCHASE_OF_KKR_TEXT?>',*/?>
		'<?=LNG_CAN_I_CALL_OFF_PURCHASE_OF_ITEMS_TEXT?>',
		'<?=LNG_CAN_I_CALL_OFF_TRADE_OF_ITEMS_SELLER_TEXT?>',
		'<?=LNG_CAN_I_CALL_OFF_TRADE_OF_ITEMS_BUYER_TEXT?>',
		'<?=LNG_GIVE_AWAY_KKR_TEXT?>',
		'<?=LNG_HOW_CAN_I_PAY_TEXT?>'<?/*,
		'<?=LNG_HOW_DOES_MOBILE_PAYMENT_WORK_TEXT?>',
		'<?=LNG_MOBILE_PAYMENT_SPAM_TEXT?>',
		'<?=LNG_WHICH_MOBILE_PROVIDERS_SUPPORTED_TEXT?>'*/?>);
FAQ.klunker_bullets = new Array();

<? /**
	* Diese zwei Funktionen werden benutzt, um die CollapseContainer unten
	* um das Wissen um ihre Kinder zu erweitern, und diese Kinder dann gemeinsam
	* alle zuzuklappen.
	*
	* Wird "except" angegeben, so wird dieses Kind nicht collapsed.
	*
	* @param CollapseContainer but
	*/ ?>
FAQ.CollapseContainer_collapse_all_children = function(except) {
	var n = this.children.length;
	for (var i = 0; i < n; i++) {
		if (this.children[i] == except)
			continue;
		this.children[i].collapse_body();
	}
};

FAQ.build_navigation = function()
{
	FAQ.c_container = new PureFW.Container(
		FAQ.this_window,
		40, 0,
		360, 430);

	FAQ.c_container.children = new Array();
	FAQ.c_container.collapse_all_children =
		FAQ.CollapseContainer_collapse_all_children;

		FAQ.general_collapser = new PureFW.CollapseContainer(
			FAQ.c_container.get_content_node(),
			0, 0,
			370, 0);
		FAQ.general_collapser.set_positioning('relative');
		FAQ.general_collapser.root_container.set_bg_img(
			'ui/elements/texts/<?=$LNG?>/FAQ/txt_general_questions.png');
		FAQ.general_collapser.root_container.set_width(247);
		FAQ.general_collapser.root_container.set_height(18);
		FAQ.general_collapser.root_container.set_padding_bottom(5);
		FAQ.c_container.children.push(FAQ.general_collapser);

		FAQ.general_collapser.children = new Array();
		FAQ.general_collapser.collapse_all_children =
			FAQ.CollapseContainer_collapse_all_children;
		FAQ.general_collapser.add_event_handler(
			"expand",
			(function (_instance) {
				return function(ev) {
					FAQ.c_container.collapse_all_children(_instance);
				}
			})(FAQ.general_collapser)
		);

		for(var i = 0; i < FAQ.general_texts.length; i++)
		{
			FAQ.general_bullets[i] = new PureFW.TextBullet(
				FAQ.general_collapser.get_body());
			FAQ.general_bullets[i].set_content(FAQ.general_questions[i]);
			FAQ.general_bullets[i].add_event_handler(
				"click",
				(function(_i) {
					return function(ev) {
						FAQ.build_current_text(
							FAQ.general_questions,
							FAQ.general_texts,
							_i
						);
					}
				})(i)
			);
		}

		FAQ.game_collapser = new PureFW.CollapseContainer(
			FAQ.c_container.get_content_node(),
			0, 0,
			370, 0);
		FAQ.game_collapser.set_positioning('relative');
		FAQ.game_collapser.root_container.set_bg_img(
			'ui/elements/texts/<?=$LNG?>/FAQ/txt_the_game.png');
		FAQ.game_collapser.root_container.set_width(247);
		FAQ.game_collapser.root_container.set_height(18);
		FAQ.game_collapser.root_container.set_padding_bottom(5);
		FAQ.c_container.children.push(FAQ.game_collapser);

		FAQ.game_collapser.children = new Array();
		FAQ.game_collapser.collapse_all_children =
			FAQ.CollapseContainer_collapse_all_children;
		FAQ.game_collapser.add_event_handler(
			"expand",
			(function (_instance) {
				return function(ev) {
					FAQ.c_container.collapse_all_children(_instance);
				}
			})(FAQ.game_collapser)
		);

			FAQ.game_before_collapser = new PureFW.CollapseContainer(
				FAQ.game_collapser.get_body(),
				0, 0,
				350, 0);
			FAQ.game_before_collapser.set_positioning('relative');
			FAQ.game_before_collapser.set_arrow('small');
			FAQ.game_before_collapser.set_font_color('black');
			FAQ.game_before_collapser.root_container.set_content(
				'<?=LNG_BEFORE_START_SUBTOPIC?>'
			);
			for(var i = 0; i < FAQ.game_before_questions.length; i++)
			{
				FAQ.game_before_bullets[i] = new PureFW.TextBullet(
					FAQ.game_before_collapser.get_body()
				);
				FAQ.game_before_bullets[i].set_content(
					FAQ.game_before_questions[i]);
				FAQ.game_before_bullets[i].add_event_handler(
					"click",
					(function(_i) {
						return function(ev) {
							FAQ.build_current_text(
								FAQ.game_before_questions,
								FAQ.game_before_texts,
								_i
							);
						}
					})(i)
				);
			}
			FAQ.game_collapser.children.push(
				FAQ.game_before_collapser
			);
			FAQ.game_before_collapser.add_event_handler(
				"expand",
				(function (_instance) {
					return function(ev) {
						FAQ.game_collapser.collapse_all_children(_instance);
					}
				})(FAQ.game_before_collapser)
			);

			FAQ.game_char_collapser = new PureFW.CollapseContainer(
				FAQ.game_collapser.get_body(),
				0, 0,
				350, 0);
			FAQ.game_char_collapser.set_positioning('relative');
			FAQ.game_char_collapser.set_arrow('small');
			FAQ.game_char_collapser.set_font_color('black');
			FAQ.game_char_collapser.root_container.set_content(
				'<?=LNG_CHARACTER_SUBTOPIC?>');
			for(var i = 0; i < FAQ.game_char_questions.length; i++)
			{
				FAQ.game_char_bullets[i] = new PureFW.TextBullet(
					FAQ.game_char_collapser.get_body());
				FAQ.game_char_bullets[i].set_content(
					FAQ.game_char_questions[i]);
				FAQ.game_char_bullets[i].add_event_handler(
					"click",
					(function(_i) {
						return function(ev) {
							FAQ.build_current_text(
								FAQ.game_char_questions,
								FAQ.game_char_texts,
								_i
							);
						}
					})(i)
				);
			}
			FAQ.game_collapser.children.push(
				FAQ.game_char_collapser
			);
			FAQ.game_char_collapser.add_event_handler(
				"expand",
				(function (_instance) {
					return function(ev) {
						FAQ.game_collapser.collapse_all_children(_instance);
					}
				})(FAQ.game_char_collapser)
			);

		FAQ.task_collapser = new PureFW.CollapseContainer(
			FAQ.c_container.get_content_node(),
			0, 0,
			370, 0);
		FAQ.task_collapser.set_positioning('relative');
		FAQ.task_collapser.root_container.set_bg_img(
			'ui/elements/texts/<?=$LNG?>/FAQ/txt_your_tasks.png');
		FAQ.task_collapser.root_container.set_width(247);
		FAQ.task_collapser.root_container.set_height(18);
		FAQ.task_collapser.root_container.set_padding_bottom(5);
		FAQ.c_container.children.push(FAQ.task_collapser);

		FAQ.task_collapser.children = new Array();
		FAQ.task_collapser.collapse_all_children =
			FAQ.CollapseContainer_collapse_all_children;
		FAQ.task_collapser.add_event_handler(
			"expand",
			(function (_instance) {
				return function(ev) {
					FAQ.c_container.collapse_all_children(_instance);
				}
			})(FAQ.task_collapser)
		);

			FAQ.task_research_collapser = new PureFW.CollapseContainer(
				FAQ.task_collapser.get_body(),
				0, 0,
				350, 0);
			FAQ.task_research_collapser.set_positioning('relative');
			FAQ.task_research_collapser.set_arrow('small');
			FAQ.task_research_collapser.set_font_color('black');
			FAQ.task_research_collapser.root_container.set_content(
				'<?=LNG_RESEARCH_SUBTOPIC?>');
			for(var i = 0; i < FAQ.task_research_questions.length; i++)
			{
				FAQ.task_research_bullets[i] = new PureFW.TextBullet(
					FAQ.task_research_collapser.get_body());
				FAQ.task_research_bullets[i].set_content(
					FAQ.task_research_questions[i]);
				FAQ.task_research_bullets[i].add_event_handler(
					"click",
					(function(_i) {
						return function(ev) {
							FAQ.build_current_text(
								FAQ.task_research_questions,
								FAQ.task_research_texts,
								_i
							);
						}
					})(i)
				);
			}
			FAQ.task_collapser.children.push(
				FAQ.task_research_collapser
			);
			FAQ.task_research_collapser.add_event_handler(
				"expand",
				(function (_instance) {
					return function(ev) {
						FAQ.task_collapser.collapse_all_children(_instance);
					}
				})(FAQ.task_research_collapser)
			);

			FAQ.task_explore_collapser = new PureFW.CollapseContainer(
				FAQ.task_collapser.get_body(),
				0, 0,
				350, 0);
			FAQ.task_explore_collapser.set_positioning('relative');
			FAQ.task_explore_collapser.set_arrow('small');
			FAQ.task_explore_collapser.set_font_color('black');
			FAQ.task_explore_collapser.root_container.set_content(
				'<?=LNG_DISCOVER_SUBTOPIC?>');
			for(var i = 0; i < FAQ.task_explore_questions.length; i++)
			{
				FAQ.task_explore_bullets[i] = new PureFW.TextBullet(
					FAQ.task_explore_collapser.get_body());
				FAQ.task_explore_bullets[i].set_content(
					FAQ.task_explore_questions[i]);
				FAQ.task_explore_bullets[i].add_event_handler(
					"click",
					(function(_i) {
						return function(ev) {
							FAQ.build_current_text(
								FAQ.task_explore_questions,
								FAQ.task_explore_texts,
								_i
							);
						}
					})(i)
				);
			}
			FAQ.task_collapser.children.push(
				FAQ.task_explore_collapser
			);
			FAQ.task_explore_collapser.add_event_handler(
				"expand",
				(function (_instance) {
					return function(ev) {
						FAQ.task_collapser.collapse_all_children(_instance);
					}
				})(FAQ.task_explore_collapser)
			);

			FAQ.task_build_collapser = new PureFW.CollapseContainer(
				FAQ.task_collapser.get_body(),
				0, 0,
				350, 0);
			FAQ.task_build_collapser.set_positioning('relative');
			FAQ.task_build_collapser.set_arrow('small');
			FAQ.task_build_collapser.set_font_color('black');
			FAQ.task_build_collapser.root_container.set_content(
				'<?=LNG_CONSTRUCT_SUBTOPIC?>');
			for(var i = 0; i < FAQ.task_build_questions.length; i++)
			{
				FAQ.task_build_bullets[i] = new PureFW.TextBullet(
					FAQ.task_build_collapser.get_body());
				FAQ.task_build_bullets[i].set_content(
					FAQ.task_build_questions[i]);
				FAQ.task_build_bullets[i].add_event_handler(
					"click",
					(function(_i) {
						return function(ev) {
							FAQ.build_current_text(
								FAQ.task_build_questions,
								FAQ.task_build_texts,
								_i
							);
						}
					})(i)
				);
			}
			FAQ.task_collapser.children.push(
				FAQ.task_build_collapser
			);
			FAQ.task_build_collapser.add_event_handler(
				"expand",
				(function (_instance) {
					return function(ev) {
						FAQ.task_collapser.collapse_all_children(_instance);
					}
				})(FAQ.task_build_collapser)
			);

			FAQ.task_buy_collapser = new PureFW.CollapseContainer(
				FAQ.task_collapser.get_body(),
				0, 0,
				350, 0);
			FAQ.task_buy_collapser.set_positioning('relative');
			FAQ.task_buy_collapser.set_arrow('small');
			FAQ.task_buy_collapser.set_font_color('black');
			FAQ.task_buy_collapser.root_container.set_content(
				'<?=LNG_TRADING_SUBTOPIC?>');
			for(var i = 0; i < FAQ.task_buy_questions.length; i++)
			{
				FAQ.task_buy_bullets[i] = new PureFW.TextBullet(
					FAQ.task_buy_collapser.get_body());
				FAQ.task_buy_bullets[i].set_content(FAQ.task_buy_questions[i]);
				FAQ.task_buy_bullets[i].add_event_handler(
					"click",
					(function(_i) {
						return function(ev) {
							FAQ.build_current_text(
								FAQ.task_buy_questions,
								FAQ.task_buy_texts,
								_i
							);
						}
					})(i)
				);
			}
			FAQ.task_collapser.children.push(
				FAQ.task_buy_collapser
			);
			FAQ.task_buy_collapser.add_event_handler(
				"expand",
				(function (_instance) {
					return function(ev) {
						FAQ.task_collapser.collapse_all_children(_instance);
					}
				})(FAQ.task_buy_collapser)
			);

			FAQ.task_treasure_collapser = new PureFW.CollapseContainer(
				FAQ.task_collapser.get_body(),
				0, 0,
				350, 0);
			FAQ.task_treasure_collapser.set_positioning('relative');
			FAQ.task_treasure_collapser.set_arrow('small');
			FAQ.task_treasure_collapser.set_font_color('black');
			FAQ.task_treasure_collapser.root_container.set_content(
				'<?=LNG_ITEMS_SUBTOPIC?>');
			for(var i = 0; i < FAQ.task_treasure_questions.length; i++)
			{
				FAQ.task_treasure_bullets[i] = new PureFW.TextBullet(
					FAQ.task_treasure_collapser.get_body());
				FAQ.task_treasure_bullets[i].set_content(
					FAQ.task_treasure_questions[i]);
				FAQ.task_treasure_bullets[i].add_event_handler(
					"click",
					(function(_i) {
						return function(ev) {
							FAQ.build_current_text(
								FAQ.task_treasure_questions,
								FAQ.task_treasure_texts,
								_i
							);
						}
					})(i)
				);
			}
			FAQ.task_collapser.children.push(
				FAQ.task_treasure_collapser
			);
			FAQ.task_treasure_collapser.add_event_handler(
				"expand",
				(function (_instance) {
					return function(ev) {
						FAQ.task_collapser.collapse_all_children(_instance);
					}
				})(FAQ.task_treasure_collapser)
			);

		FAQ.klunker_collapser = new PureFW.CollapseContainer(
			FAQ.c_container.get_content_node(),
			0, 0,
			370, 0);
		FAQ.klunker_collapser.set_positioning('relative');
		FAQ.klunker_collapser.root_container.set_bg_img(
			'ui/elements/texts/<?=$LNG?>/FAQ/txt_klunker_and_charge_klunker.png');
		FAQ.klunker_collapser.root_container.set_width(247);
		FAQ.klunker_collapser.root_container.set_height(18);
		FAQ.klunker_collapser.root_container.set_padding_bottom(5);
		FAQ.c_container.children.push(FAQ.klunker_collapser);

		FAQ.klunker_collapser.children = new Array();
		FAQ.klunker_collapser.collapse_all_children =
			FAQ.CollapseContainer_collapse_all_children;
		FAQ.klunker_collapser.add_event_handler(
			"expand",
			(function (_instance) {
				return function(ev) {
					FAQ.c_container.collapse_all_children(_instance);
				}
			})(FAQ.klunker_collapser)
		);

		for(var i = 0; i < FAQ.klunker_questions.length; i++)
		{
			FAQ.klunker_bullets[i] = new PureFW.TextBullet(
				FAQ.klunker_collapser.get_body());
			FAQ.klunker_bullets[i].set_content(FAQ.klunker_questions[i]);
			FAQ.klunker_bullets[i].add_event_handler(
				"click",
				(function(_i) {
					return function(ev) {
						FAQ.build_current_text(
							FAQ.klunker_questions,
							FAQ.klunker_texts,
							_i
						);
					}
				})(i)
			);
		}
}
FAQ.build_navigation();

FAQ.info_box = new PureFW.Container(
	FAQ.this_window,
	415, 0,
	527, 432);
FAQ.info_box.set_bg_img("ui/backgrounds/faq/bg_527x432.png");

FAQ.destroy_current_texts = function()
{
	var n = FAQ.current_text_containers.length;
	for (var i = 0; i < n; i++)
	{
		FAQ.current_text_containers[i].destroy();
	}
	FAQ.info_scroller.set_inner_height(0);
}

FAQ.build_current_text = function(question_array, answer_array, item_i) {
	if (question_array.length != answer_array.length)
		throw new Error(
			"FAQ.build_current_text: different lenghts for question_array and "+
			"answer_array!"
		);
	FAQ.destroy_current_texts();
	var n = question_array.length; // number of items
	var cur_y = 0;
	var scroll_to_y = 0;
	for(var i = 0; i < n; i++)
	{
		FAQ.current_text_containers[i] = new PureFW.Container(
			FAQ.info_scroller.get_content_node(),
			0, 0,
			0, 0);
		FAQ.current_text_containers[i].set_positioning('relative');
		FAQ.current_text_containers[i].set_content(question_array[i]
	        + '<br/><br/>'
			+ answer_array[i] + '<br/><br/><br/>');
		if (i == item_i) {
			<? /* Angewählte Antwort hervorheben ... */?>
			FAQ.current_text_containers[i].set_font_weight('bold');
			<? /* ... und dorthin scrollen (passiert später)*/ ?>
			scroll_to_y = cur_y;		<?/* padding abziehn*/?>
		}
		cur_y += FAQ.current_text_containers[i].get_height()
			/ FAQ.current_text_containers[i].scale_factor;
	}

	FAQ.info_scroller.update_inner_height();
	FAQ.info_scroller.scroll_to(0, scroll_to_y);
}

FAQ.info_scroller = new PureFW.ScrollContainer(
	FAQ.info_box.get_content_node(),
	20, 20,
	FAQ.info_box.width - 40, FAQ.info_box.height - 40);
FAQ.info_scroller.set_inner_height(0);



FAQ.build_current_text(FAQ.general_questions, FAQ.general_texts);



FAQ.destroy = function()
{
	FAQ.this_window.remove_event_handler("destroy", FAQ.destroy);
	FAQ.this_window.remove_event_handler("change", FAQ.destroy);
	for (var x in FAQ) {
		<?/* Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		   * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		   * Wir haben hier nicht die Berechtigung dort einzugreifen!*/?>
		if (x == 'this_window')
			continue;
		try {
			FAQ[x].destroy();
		}
		catch (e) {
			delete FAQ[x];
		}
	}
	delete FAQ;
	if (PureFW.Officer.get_current_active_officer())
		PureFW.Officer.get_current_active_officer().show();
};

FAQ.this_window.add_event_handler("destroy", FAQ.destroy);
FAQ.this_window.add_event_handler("change", FAQ.destroy);
</script>