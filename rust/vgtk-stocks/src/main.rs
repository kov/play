use vgtk::ext::*;
use vgtk::lib::gio::ApplicationFlags;
use vgtk::lib::gtk::*;
use vgtk::{gtk, run, Component, UpdateAction, VNode};

#[derive(Clone, Debug)]
struct Stock {
    ticker: String,
}

impl Stock {
    fn new<S: ToString>(ticker: S) -> Self {
        Self {
            ticker: ticker.to_string(),
        }
    }

    fn render(&self) -> VNode<Model> {
        gtk! {
            <ListBoxRow>
                <Label label=self.ticker.clone() />
            </ListBoxRow>
        }
    }
}

#[derive(Clone, Debug)]
struct Model {
    stocks: Vec<Stock>,
}

impl Default for Model {
    fn default() -> Self {
        Self {
            stocks: vec![
                Stock::new("PETR4"),
                Stock::new("VALE3"),
                Stock::new("JSLG3"),
                Stock::new("ANIM3"),
            ],
        }
    }
}

#[derive(Clone, Debug)]
enum Message {
    Exit,
}

impl Component for Model {
    type Message = Message;
    type Properties = ();

    fn update(&mut self, msg: Self::Message) -> UpdateAction<Self> {
        match msg {
            Message::Exit => {
                vgtk::quit();
                UpdateAction::None
            }
        }
    }

    fn view(&self) -> VNode<Model> {
        gtk! {
            <Application::new_unwrap(Some("com.example.vgtk-todomvc"), ApplicationFlags::empty())>
                <Window border_width=20 on destroy=|_| Message::Exit>
                    <Paned orientation=Orientation::Horizontal>
                        <ListBox>
                        {
                            self.stocks.iter().map(Stock::render)
                        }
                        </ListBox>
                        <Label label="kovR"/>
                    </Paned>
                </Window>
            </Application>
        }
    }
}

fn main() {
    pretty_env_logger::init();
    std::process::exit(run::<Model>());
}
