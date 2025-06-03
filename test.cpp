#include <SFML/Graphics.hpp>

int main()
{
  // Create a window with title and dimensions
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

  // Define a blue color
  sf::Color blue(0, 0, 255);

  // Main loop that keeps the window open
  while (window.isOpen())
  {
    // Check for events like closing the window
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    // Clear the window with blue color
    window.clear(blue);

    // Display the window contents
    window.display();
  }

  return 0;
}
