#include "TextEntry.hpp"

TextEntry::TextEntry(const std::string &placeHolder, const sf::FloatRect &pos, const std::shared_ptr<sf::Sprite> &back)
: _pos(pos), _use(false), _text(""), _first(true), _isHover(false), _background(back),
  _displayText(), _placeHolder(placeHolder)
{
}

TextEntry::~TextEntry()
{
  
}

void  TextEntry::setFont(const sf::Font &font)
{
  _displayText.setFont(font);
}

void  TextEntry::setCharacterSize(unsigned int size)
{
  _displayText.setCharacterSize(size);
}

void  TextEntry::setTextColor(const sf::Color &color)
{
  _displayText.setColor(color);
}

const std::string &TextEntry::getText() const
{
  return (_text);
}

TextEntry &TextEntry::operator+=(char c)
{
  _text += c;
  return (*this);
}

TextEntry &TextEntry::operator+=(const std::string &str)
{
  _text += str;
  return (*this);
}

void  TextEntry::draw(sf::RenderWindow &win)
{
  sf::View  text(_pos);

  text.setViewport(sf::FloatRect(0.f, 0.f, _pos.width / win.getSize().x, _pos.height / win.getSize().y));
  win.setView(text);
  auto size = _background->getTextureRect();
  _background->setScale(_pos.width / size.width, _pos.height / size.height);
  _background->setPosition({_pos.left, _pos.top});
  _displayText.setPosition({_pos.left, _pos.top});
  if (_first)
    _displayText.setString(_placeHolder);
  else
    _displayText.setString(_text);
  if (_displayText.getLocalBounds().width > _pos.width)
    _displayText.move(_pos.width - _displayText.getLocalBounds().width, 0);
  win.draw(*_background);
  win.draw(_displayText);
  win.setView(win.getDefaultView());
}

void  TextEntry::update(const sf::Event &event)
{
  if (event.type == sf::Event::MouseMoved)
  {
    if (event.mouseMove.x >= _pos.left && event.mouseMove.x <= _pos.left + _pos.width
        && event.mouseMove.y >= _pos.top && event.mouseMove.y <= _pos.top + _pos.height)
      _isHover = true;
    else
      _isHover = false;
  }
  if (_isHover && event.type == sf::Event::MouseButtonReleased)
  {
    _use = true;
    _first = false;
  }
  else if (!_isHover && event.type == sf::Event::MouseButtonReleased && _text.empty())
    _first = true;
  if (_use && event.type == sf::Event::KeyPressed)
  {
    if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
      _text += static_cast<char>(event.key.code + (event.key.shift == true ? 65 : 97));
    else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
      _text += static_cast<char>(event.key.code + 22);
    else if (event.key.code == sf::Keyboard::BackSpace)
      _text.pop_back();
  }

}
