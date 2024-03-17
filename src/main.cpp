#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;


bool follow_player;

class $modify(GJBaseGameLayer) {
	void updateCamera(float p0) {
		if (!LevelEditorLayer::get() || follow_player) return GJBaseGameLayer::updateCamera(p0);
	}
};

class $modify(MyEditorPauseLayer, EditorPauseLayer) {
	bool init(LevelEditorLayer *p0) {
		if (!EditorPauseLayer::init(p0)) return false;
		CCMenu *options_menu = static_cast<CCMenu*>(getChildByID("options-menu"));
		CCMenuItemToggler *toggle = CCMenuItemToggler::createWithStandardSprites(
			this, menu_selector(MyEditorPauseLayer::onFollowPlayerToggle), 0.55
		);
		toggle->toggle(follow_player);

		CCLabelBMFont *some_other_text = getChild<CCLabelBMFont>(options_menu, -1);
		
		CCLabelBMFont *text = CCLabelBMFont::create("Follow Player", "bigFont.fnt");

		text->setLayoutOptions(some_other_text->getLayoutOptions());

		options_menu->addChild(toggle);
		options_menu->addChild(text);
		options_menu->updateLayout();

		return true;
	}

	void onFollowPlayerToggle(CCObject *sender) {
		follow_player = !static_cast<CCMenuItemToggler *>(sender)->isToggled();
		Mod::get()->setSavedValue("follow-player", follow_player);
	}
};

$on_mod(Loaded) {
	follow_player = Mod::get()->getSavedValue<bool>("follow-player", true);
}
