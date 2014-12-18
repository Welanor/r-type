#include <algorithm>
#include <sstream>
#include "ListBox.hpp"

ListBox::ListBox(const sf::FloatRect &pos, std::vector<t_room> &vec, std::function<void (uint32_t)> func)
: ADrawable(false, {pos.left, pos.top}, {pos.width, pos.height}), _list(vec), _cam(_pos.y), _func(func)
{
  if (!_list.empty())
    updateEntry();
}

ListBox::~ListBox()
{
  
}

void  ListBox::update(const sf::Event &event)
{
  updateEntry();
  for (auto it : _items)
    it.second->update(event);
  if (event.type == sf::Event::MouseWheelMoved
      && event.mouseWheel.x >= _pos.x && event.mouseWheel.x < _pos.x + _size.x
      && event.mouseWheel.y >= _pos.y && event.mouseWheel.y < _pos.y + _size.y)
  {
    float tmp = _cam + (event.mouseWheel.delta * 2);

    if (tmp <= _pos.y && tmp >= (_pos.y + _size.y) - (_items.size() * 50) - _pos.y)
      _cam = tmp;
  }
}

void  ListBox::draw(sf::RenderWindow &win)
{
  float y = _cam;
  sf::View  view({_pos.x, _pos.y, _size.x, _size.y});

  view.setViewport({ _pos.x / win.getSize().x, _pos.y / win.getSize().y, _size.x / win.getSize().x, _size.y / win.getSize().y});
  win.setView(view);
  for (auto it : _items)
  {
    it.second->setPosition({_pos.x, y});
    it.second->draw(win);
    y += it.second->getSize().y;
  }
  win.setView(win.getDefaultView());
}

void  ListBox::updateEntry()
{
  std::vector<t_room> toAdd;
  std::vector<t_room> toRemove;
  
  std::set_difference(_list.begin(), _list.end(), _displayRoom.begin(), _displayRoom.end(),
                      std::inserter(toAdd, toAdd.begin()), [](const t_room &a, const t_room &b) -> bool { return a.id != b.id; });
  
  for (auto it : toAdd)
  {
    std::stringstream ss("");

    ss << static_cast<int>(it.nbPlayer) << " / " << static_cast<int>(it.playerMax);
    std::shared_ptr<Text>  name(new Text({0, 0, 100, 100}, it.name));
    std::shared_ptr<Text>  player(new Text({200, 0, 100, 100}, ss.str()));
    
    name->setCharacterSize(30);
    player->setCharacterSize(30);
    
    name->setColor(sf::Color::Black);
    player->setColor(sf::Color::Black);

    name->setFont(*RessourceManager::instance().getFont("../assets/font.ttf"));
    player->setFont(*RessourceManager::instance().getFont("../assets/font.ttf"));
    
    auto texture = RessourceManager::instance().getTexture("../assets/ListEntry.png");
    
    std::shared_ptr<sf::Sprite>  button(new sf::Sprite(*texture));
    std::shared_ptr<sf::Sprite>  hover(new sf::Sprite(*texture));
    std::shared_ptr<sf::Sprite>  click(new sf::Sprite(*texture));
    
    button->setTextureRect(sf::IntRect(0, 0, 480, 30));
    hover->setTextureRect(sf::IntRect(0, 30, 480, 30));
    click->setTextureRect(sf::IntRect(0, 60, 480, 30));
    
    std::shared_ptr<Button> tmp(new Button({ 0, 0 , _size.x, 50 }, button, hover, click));
    
    tmp->onClick([this, it] {
      _func(it.id);
      std::cout << "Join [" << it.name << "]" << std::endl;
    });
    _items[it.id] = std::shared_ptr<ListItem>(new ListItem({_size.x, 50},
                                                           name, player, tmp));
    _displayRoom.push_back(it);
  }

  std::set_difference(_displayRoom.begin(), _displayRoom.end(), _list.begin(), _list.end(),
                      std::inserter(toRemove, toRemove.begin()), [](const t_room &a, const t_room &b) -> bool { return a.id != b.id; });

  for (auto it : toRemove)
    _displayRoom.erase(std::remove_if(_displayRoom.begin(), _displayRoom.end(), [&it](const t_room &a) -> bool {
      return (a.id == it.id);
    }), _displayRoom.end());
}