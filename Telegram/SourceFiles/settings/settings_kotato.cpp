/*
This file is part of Kotatogram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/kotatogram/kotatogram-desktop/blob/dev/LEGAL
*/
#include "settings/settings_kotato.h"

#include "settings/settings_common.h"
#include "settings/settings_chat.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "ui/widgets/continuous_sliders.h"
#include "ui/text/text_utilities.h" // Ui::Text::ToUpper
#include "boxes/connection_box.h"
#include "boxes/fonts_box.h"
#include "boxes/net_boost_box.h"
#include "boxes/about_box.h"
#include "boxes/confirm_box.h"
#include "platform/platform_specific.h"
#include "window/window_session_controller.h"
#include "lang/lang_keys.h"
#include "core/update_checker.h"
#include "core/kotato_settings.h"
#include "core/application.h"
#include "storage/localstorage.h"
#include "data/data_session.h"
#include "main/main_session.h"
#include "layout.h"
#include "facades.h"
#include "app.h"
#include "styles/style_settings.h"

namespace Settings {

void SetupKotatoChats(not_null<Ui::VerticalLayout*> container) {
	AddSkip(container);
	AddSubsectionTitle(container, tr::ktg_settings_chats());

	AddButton(
		container,
		tr::ktg_settings_emoji_outline(),
		st::settingsButton
	)->toggleOn(
		rpl::single(BigEmojiOutline())
	)->toggledValue(
	) | rpl::filter([](bool enabled) {
		return (enabled != BigEmojiOutline());
	}) | rpl::start_with_next([](bool enabled) {
		SetBigEmojiOutline(enabled);
		KotatoSettings::Write();
	}, container->lifetime());

	AddButton(
		container,
		tr::ktg_settings_chat_list_compact(),
		st::settingsButton
	)->toggleOn(
		rpl::single(DialogListLines() == 1)
	)->toggledValue(
	) | rpl::filter([](bool enabled) {
		return (enabled != (DialogListLines() == 1));
	}) | rpl::start_with_next([](bool enabled) {
		SetDialogListLines(enabled ? 1 : 2);
		KotatoSettings::Write();
	}, container->lifetime());

	AddButton(
		container,
		tr::ktg_settings_always_show_scheduled(),
		st::settingsButton
	)->toggleOn(
		rpl::single(cAlwaysShowScheduled())
	)->toggledValue(
	) | rpl::filter([](bool enabled) {
		return (enabled != cAlwaysShowScheduled());
	}) | rpl::start_with_next([](bool enabled) {
		cSetAlwaysShowScheduled(enabled);
		Notify::showScheduledButtonChanged();
		KotatoSettings::Write();
	}, container->lifetime());

	AddButton(
		container,
		tr::ktg_settings_fonts(),
		st::settingsButton
	)->addClickHandler([=] {
		Ui::show(Box<FontsBox>());
	});


	AddSkip(container);
}

Kotato::Kotato(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent) {
	setupContent(controller);
}

void Kotato::setupContent(not_null<Window::SessionController*> controller) {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

	SetupKotatoChats(content);

	Ui::ResizeFitChild(this, content);
}

} // namespace Settings

