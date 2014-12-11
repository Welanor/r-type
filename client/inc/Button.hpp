#ifndef _BUTTON_HPP_
# define _BUTTON_HPP_

# include <SFML/Graphics.hpp>
# include <iostream>

class Button
{
public:
  Button(const sf::FloatRect &pos,
         const std::shared_ptr<sf::Sprite> &displayTexture,
         const std::shared_ptr<sf::Sprite> &hoverTexture,
         const std::shared_ptr<sf::Sprite> &clickedTexture);
  ~Button();
  
  void  update(const sf::Event &event);
  void  draw(sf::RenderWindow &win);
  
  bool  isHidden() const;
  bool  isHover() const;
  bool  isClicked() const;

private:

  bool  _hide;
  bool  _hover;
  bool  _isClicked;

  sf::FloatRect               _pos;
  std::shared_ptr<sf::Sprite> _displayTexture;
  std::shared_ptr<sf::Sprite> _hoverTexture;
  std::shared_ptr<sf::Sprite> _clickedTexture;
};

#endif
