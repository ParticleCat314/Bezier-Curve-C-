#include <SFML/Graphics.hpp>
#include "math.h"

int scale = 30;
int x;
int y;
int z = 0;
int i = 30;
bool pressed;
bool grid_true = true;
bool view_stationary_points = true;
bool Hide_points = false;
Vec2f *selected = nullptr;
sf::Clock timeE;

int size = 800;





void draw_text(string words, int x, int y, int size , sf::RenderWindow& window){
    sf::Text text;
    sf::Font font;

    font.loadFromFile("arialbd.ttf"); // Load the font from a file. If file not present text will fail.
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setFont(font);
    text.setCharacterSize(size);
    text.setPosition(x,y);
    text.setString(words);
    window.draw(text); // Drawing the text.

}



class sfLine : public sf::Drawable // This class is taken from the internet. NOT MINE. Unable to locate original source. 
{
public:
    sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2, sf::Color color):
        thickness(8.f)
    {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i=0; i<4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(vertices,4,sf::Quads);
    }


private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
};


class curve { // Class that manages the bezier curve: Generation, drawing, etc.
    private:

        // Declare some public variables for control point postion, color, etc.
        double ax;
        double bx;
        double cx;
        double dx;  
        double ay;
        double by;
        double cy;
        double dy;
        int r;
        int g;
        int b;



    public:
        Vec2f start;
        Vec2f ending;
        Vec2f c1;
        Vec2f c2;
        std::string first_equation;  // Used to 'hold' the eqution.
        std::string second_equation; // Used to 'hold' the eqution.



        curve() {
            start = Vec2f(200,400);
            ending = Vec2f(400,400);
            c1 = Vec2f(400,200);
            c2 = Vec2f(200,200);
            // Initialised the curve. 

        }


        void coff(){ // Sets the coeffieceints of the curve based on control points.
            ax = ending.x/scale-(3*c2.x/scale)+(3*c1.x/scale)-start.x/scale;
            bx = (3*c2.x/scale)-(6*c1.x/scale)+(3*start.x)/scale;
            cx = (3*c1.x/scale)-(3*start.x/scale);
            dx = start.x/scale;

            ay = (ending.y/scale)-(3*c2.y/scale)+(3*c1.y/scale)-start.y/scale;
            by = (3*c2.y/scale)-(6*c1.y/scale)+(3*start.y/scale);
            cy = (3*c1.y/scale)-(3*start.y/scale);
            dy = start.y/scale;

        }

        void draw_curve(sf::RenderWindow& window,float time){  // Draw the curve, pretty self explanatory.
            // Initialise some local variables for the drawing loop:
            double t = 0;
            float prevx = ax*pow(t,3) + bx*pow(t,2) + cx*t + dx; // Set start of curve, x axis.
            float prevy = ay*pow(t,3) + by*pow(t,2) + cy*t + dy; // Set start of cruve, y axis.
            float nextx = 0;
            float nexty = 0;
            float ittr = 300;    // Number of interations. Increse for smoother curve.
            float step = 1/ittr; // Set the step size based on itterations.
            float mul = 0.05;    // Arbitrary value for color multiplier.

            z = 0;
            for (int n = 0;n<=ittr;n++){ // Main curve drawing loop. Draws extremly small line segments following curve.
                r = 124.5*sin(mul*z-time)+124.5;         // Calculate color gradient red
                g = 124.5*sin(mul*z-0.6*PI-time)+124.5;  // Calculate color gradient green
                b = 124.5*sin(mul*z-1.2*PI-time)+124.5;  // Calculate color gradient blue
                sf::Vector2f q(sf::Vector2f(scale*prevx,(-scale*prevy)+size));
                nextx = ax*pow(t,3) + bx*pow(t,2) + cx*t + dx;
                nexty = ay*pow(t,3) + by*pow(t,2) + cy*t + dy;
                sf::Vector2f w(sf::Vector2f(scale*nextx,(-scale*nexty)+size));
                prevx = nextx;
                prevy = nexty;
                t += step;
                z++;
                sfLine a(q,w,sf::Color(r,g,b));

                a.draw(window,sf::RenderStates::Default);

            }


        }

        // Draw all the neccessary points-of-interest.
        void draw_points(sf::RenderWindow& window){ 
            // Drawing control points.
            sf::CircleShape circle1;
            sf::CircleShape circle2;
            sf::CircleShape circle3;
            sf::CircleShape circle4;
            float radius = 8;
            float diff = size-radius;
            circle1.setRadius(radius);
            circle1.setPosition(start.x-10,-start.y+diff);
            circle1.setFillColor(sf::Color::Green);
            circle2.setRadius(radius);
            circle2.setPosition(c1.x-10,-c1.y+diff);
            circle2.setFillColor(sf::Color::Yellow);
            circle3.setRadius(radius);
            circle3.setPosition(c2.x-10,-c2.y+diff);
            circle3.setFillColor(sf::Color::Yellow);
            circle4.setRadius(radius);
            circle4.setPosition(ending.x-10,-ending.y+diff);
            circle4.setFillColor(sf::Color::Red);
            window.draw(circle1);
            window.draw(circle2);
            window.draw(circle3);
            window.draw(circle4);


        }


        void eprint(sf::RenderWindow& window ){ // Prints the equations and other values:
            first_equation = "x = " + roundy(ax) + "t^3" + " + " + roundy(bx) + "t^2" + " + " + roundy(cx) + "t" + " + " + roundy(dx); 
            draw_text(first_equation,25,0,16,window);
            second_equation = "y = " + roundy(ay) + "t^3" + " + " + roundy(by) + "t^2" + " + " + roundy(cy) + "t" + " + " + roundy(dy); 
            draw_text(second_equation,25,20,16,window);
            std::string a;
            a = "(" + roundy((c1.x/scale)) + "," + roundy((c1.y/scale)) + ")"; 
            draw_text(a,c1.x,size-c1.y,14,window);
            a = "(" + roundy((c2.x/scale)) + "," + roundy((c2.y/scale)) + ")"; 
            draw_text(a,c2.x,size-c2.y,14,window);
            a = "(" + roundy((start.x/scale)) + "," + roundy((start.y/scale)) + ")"; 
            draw_text(a,start.x,size-start.y,14,window);
            a = "(" + roundy((ending.x/scale)) + "," + roundy((ending.y/scale)) + ")"; 
            draw_text(a,ending.x,size-ending.y,14,window);
        }



        void max(sf::RenderWindow& window){ // Calculate local maximum. WORK IN PROGRESS.
            std::tuple<float,float> x = quadratic(3*ay,2*by,cy);
            
            float t = std::get<0>(x);

            if (0<=t && t<=1){
                float stationary1x = ax*pow(t,3)+bx*pow(t,2)+cx*t+dx;
                float stationary1y = ay*pow(t,3)+by*pow(t,2)+cy*t+dy;
                sf::CircleShape station;
                station.setRadius(5);
                station.setPosition((int)(stationary1x*scale),(int)(-stationary1y*scale+size-5));
                station.setFillColor(sf::Color::Blue);
                window.draw(station);
            }

            t = std::get<1>(x);

            if (0<=t && t<=1){
                float stationary2x = ax*pow(t,3)+bx*pow(t,2)+cx*t+dx;
                float stationary2y = ay*pow(t,3)+by*pow(t,2)+cy*t+dy;
                sf::CircleShape station2;
                station2.setRadius(5);
                station2.setPosition((int)(stationary2x*scale),(int)(-stationary2y*scale+size-5));
                station2.setFillColor(sf::Color::Blue); 
                window.draw(station2);
                      
            }       
        }
        void move(sf::Event event,int x, int y){ // Just manages the movement of the points.
            //selected = nullptr;
            if (event.type == sf::Event::MouseButtonPressed & event.mouseButton.button == sf::Mouse::Left)  {  pressed = true; 
                if (sqrt(pow((start.x-x),2) + pow(size-start.y-y,2)) < 30) {selected = &start; goto stretch;}
                if (sqrt(pow((c1.x-x),2) + pow(size-c1.y-y,2)) < 30) {selected = &c1; goto stretch;}
                if (sqrt(pow((c2.x-x),2) + pow(size-c2.y-y,2)) < 30) {selected = &c2; goto stretch;}
                if (sqrt(pow((ending.x-x),2) + pow(size-ending.y-y,2)) < 30) {selected = &ending; goto stretch;}
             }
            if (event.type == sf::Event::MouseButtonReleased & event.mouseButton.button == sf::Mouse::Left) {  pressed = false; selected = nullptr; }
            stretch:
            if (selected != NULL & pressed==true) {*selected = Vec2f(x,size-y);}

        }

        void lines(sf::RenderWindow& window){ // Draw a line start-control1 and end-control1.
            sf::Vertex a [2] =  {sf::Vector2f(start.x,size-start.y),sf::Vector2f(c1.x,size-c1.y)};
            window.draw(a,2,sf::Lines);
            sf::Vertex b [2] =  {sf::Vector2f(ending.x,size-ending.y),sf::Vector2f(c2.x,size-c2.y)};
            window.draw(b,2,sf::Lines);
        }

    
};


void grid(sf::RenderWindow& window){ // Draw a grid based of the scale defined.

    int sep = size/scale;

    for (int x = 0; x<sep;x++){
        sf::Vertex thing1[2] = {sf::Vertex(sf::Vector2f(x*scale,0),sf::Color(i,i,i)),sf::Vertex(sf::Vector2f(x*scale,size),sf::Color(i,i,i))};
        sf::Vertex thing2[2] = {sf::Vertex(sf::Vector2f(0,size-x*scale),sf::Color(i,i,i)),sf::Vertex(sf::Vector2f(size,size-x*scale),sf::Color(i,i,i))};

        draw_text(to_string(x),0,size-x*scale,10,window);


        window.draw(thing2,2,sf::Lines);
        window.draw(thing1,2,sf::Lines);
    }
}


int screen_to_cart(int point){ // WIP

    return (int)point*size/scale;

}

int cart_to_screen(int point){ // WIP

    return (int)size/scale*point;
}





int main(int argc, char const *argv[])
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0f;
    sf::RenderWindow window(sf::VideoMode(size,size), "Bezier Curve thingo. Lachlan MacLean. Year 11",sf::Style::Default,settings);
    sf::View view = window.getDefaultView();
    curve bezier; // Initialise an instance of the class
    float c = 0;

    
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event)) // Handle user input for zoom and basic graph controls. WIP.
        {

            if(event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G){
                grid_true = !grid_true;

            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S){
                view_stationary_points = !view_stationary_points;
            }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H){
                Hide_points = !Hide_points;
            }          

            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z){
                scale += 10;
            }       
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X){
                scale -= 10;
            }     


            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                sf::Clipboard::setString(bezier.first_equation);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
                sf::Clipboard::setString(bezier.second_equation);
            }



        }

        x = sf::Mouse::getPosition(window).x;   // Get current mouse position x
        y = sf::Mouse::getPosition(window).y;   // Get current mouse position y


        // Animate the curve consistently:
        sf::Time Time = timeE.getElapsedTime(); 
        float t = Time.asSeconds();
        timeE.restart();

                
        window.clear();
        bezier.move(event,x,y);
        bezier.coff();
        if(grid_true == true){grid(window);}
        bezier.draw_curve(window,c);
        if (Hide_points == false){bezier.draw_points(window); bezier.eprint(window); bezier.lines(window);}
        if (view_stationary_points == true){bezier.max(window);}


        window.display();
        
        if (c>1000){c = 0;}
        

        c += 10*t;
    }
    return 0;
}
