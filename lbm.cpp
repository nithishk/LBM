#include "grid.cpp"
#include "fileReader.cpp"
#include <cstring>
#include <iostream>
#include <cmath>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define C 0
#define W 1
#define S 2
#define E 3
#define N 4
#define SW 5
#define NW 6
#define SE 7
#define NE 8


using namespace lbm;


static double t[10] = {0};
static size_t sizex;
static size_t sizey;
static size_t point_data;
static double omega;
static size_t cur_timestep = 0;

//Stream for Nomal task
void stream (PDF_Field& src , PDF_Field& dst)
{   //Corner left bottom
    dst(0 , 0 , C) = src (0 , 0 , C);
    dst(0 , 0 , E) = src (0 , 0 , W);
    dst(0 , 0 , N) = src (0 , 0 , S);
    dst(1 , 0 , E) = src (0 , 0 , E);
    dst(0 , 1 , N) = src (0 , 0 , N);
    dst(0 , 0 , NE) = src (0 , 0 , SW);
    dst(0 , 0 , SE) = src (0 , 0 , NW);
    dst(0 , 0 , NW) = src (0 , 0 , SE);
    dst(1 , 1 , NE) = src (0 , 0 , NE);

    //Corner left top
    dst(0 , sizey - 1 , C) = src (0 , sizey - 1 , C);
    dst(0 , sizey - 1 , E) = src (0 , sizey - 1 , W);
    dst(0 , sizey - 2 , S) = src (0 , sizey - 1 , S);
    dst(1 , sizey - 1 , E) = src (0 , sizey - 1 , E);
    dst(0 , sizey - 1 , S) = src (0 , sizey - 1 , N);
    dst(0 , sizey - 1 , NE) = src (0 , sizey - 1 , SW);
    dst(0 , sizey - 1 , SE) = src (0 , sizey - 1 , NW) + 0.04 / 3.0;
    dst(1 , sizey - 2 , SE) = src (0 , sizey - 1 , SE);
    dst(0 , sizey - 1 , SW) = src (0 , sizey - 1 , NE) - 0.04 / 3.0;

    //Corner right bottom
    dst(sizex - 1 , 0 , C) = src (sizex - 1 , 0 , C);
    dst(sizex - 2 , 0 , W) = src (sizex - 1 , 0 , W);
    dst(sizex - 1 , 0 , N) = src (sizex - 1 , 0 , S);
    dst(sizex - 1 , 0 , W) = src (sizex - 1 , 0 , E);
    dst(sizex - 1 , 1 , N) = src (sizex - 1 , 0 , N);
    dst(sizex - 1 , 0 , NE) = src (sizex - 1 , 0 , SW);
    dst(sizex - 2 , 1 , NW) = src (sizex - 1 , 0 , NW);
    dst(sizex - 1 , 0 , NW) = src (sizex - 1 , 0 , SE);
    dst(sizex - 1 , 0 , SW) = src (sizex - 1 , 0 , NE);

    //Corner right top
    dst(sizex - 1 , sizey - 1 , C) = src (sizex - 1 , sizey - 1 , C);
    dst(sizex - 2 , sizey - 1 , W) = src (sizex - 1 , sizey - 1 , W);
    dst(sizex - 1 , sizey - 2 , S) = src (sizex - 1 , sizey - 1 , S);
    dst(sizex - 1 , sizey - 1 , W) = src (sizex - 1 , sizey - 1 , E);
    dst(sizex - 1 , sizey - 1 , S) = src (sizex - 1 , sizey - 1 , N);
    dst(sizex - 2 , sizey - 2 , SW) = src (sizex - 1 , sizey - 1 , SW);
    dst(sizex - 1 , sizey - 1 , SE) = src (sizex - 1 , sizey - 1 , NW) + 0.04 / 3.0;
    dst(sizex - 1 , sizey - 1 , NW) = src (sizex - 1 , sizey - 1 , SE);
    dst(sizex - 1 , sizey - 1 , SW) = src (sizex - 1 , sizey - 1 , NE) - 0.04 / 3.0;

    for (size_t x = 1 ; x < sizex - 1 ; ++x)
    {   //Boundary bottom : y = 0;
        dst(x , 0 , C) = src (x , 0 , C);
        dst(x - 1, 0 , W) = src(x , 0 , W);
        dst(x , 0 , N) = src(x , 0 , S);
        dst(x + 1 , C , E) = src(x , 0 , E);
        dst(x , 1 , N) = src(x , 0 , N);
        dst(x , 0 , NE) = src(x , 0 , SW);
        dst(x - 1 , 1 , NW) = src(x , 0 , NW);
        dst(x , 0 , NW) = src(x , 0 , SE);
        dst(x + 1 , 1 , NE) = src(x , 0 , NE);
        
        //Inner fluid cells
        for (size_t y = 1 ; y < sizey - 1 ; ++y)
        {   dst(x , y , C) = src(x , y , C);
            dst(x - 1, y , W) = src(x , y , W);
            dst(x , y - 1, S) = src(x , y , S);
            dst(x + 1 , y , E) = src(x , y , E);
            dst(x , y + 1, N) = src(x , y , N);
            dst(x - 1 , y - 1 , SW) = src(x , y , SW);
            dst(x - 1 , y + 1 , NW) = src(x , y , NW);
            dst(x + 1 , y - 1 , SE) = src(x , y , SE);
            dst(x + 1 , y + 1 , NE) = src(x , y , NE);
        }

        //Boundary top : y = sizey - 1
        dst(x , sizey - 1 , C) = src (x , sizey - 1 , C);
        dst(x - 1, sizey - 1 , W) = src(x , sizey - 1 , W);
        dst(x , sizey - 2 , S) = src(x , sizey - 1 , S);
        dst(x + 1 , sizey - 1 , E) = src(x , sizey - 1 , E);
        dst(x , sizey - 1 , S) = src(x , sizey - 1 , N);
        dst(x - 1 , sizey - 2 , SW) = src(x , sizey - 1 , SW);
        dst(x , sizey - 1 , SE) = src(x , sizey - 1 , NW) + 0.04 / 3.0;
        dst(x + 1 , sizey - 2 , SE) = src(x , sizey - 1 , SE);
        dst(x , sizey - 1 , SW) = src(x , sizey - 1 , NE) - 0.04 / 3.0;
    }

    for (size_t y = 1 ; y < sizey - 1 ; ++y)
    {   //Boundary left : x = 0
        dst(0 , y , C) = src(0 , y , C);
        dst(0 , y , E) = src(0 , y , W);
        dst(0 , y - 1 , S) = src(0 , y , S);
        dst(1 , y , E) = src(0 , y , E);
        dst(0 , y + 1 , N) = src(0 , y , N);
        dst(0 , y , NE) = src(0 , y , SW);
        dst(0 , y , SE) = src(0 , y , NW);
        dst(1 , y - 1 , SE) = src(0 , y , SE);
        dst(1 , y + 1 , NE) = src(0 , y , NE);

        //Boundary right : x = sizex - 1
        dst(sizex - 1 , y , C) = src(sizex - 1 , y , C);
        dst(sizex - 2 , y , W) = src(sizex - 1 , y , W);
        dst(sizex - 1 , y - 1 , S) = src(sizex - 1 , y , S);
        dst(sizex - 1 , y , W) = src(sizex - 1 , y , E);
        dst(sizex - 1 , y + 1 , N) = src(sizex - 1 , y , N);
        dst(sizex - 2 , y - 1 , SW) = src(sizex - 1 , y , SW);
        dst(sizex - 2 , y + 1 , NW) = src(sizex - 1 , y , NW);
        dst(sizex - 1 , y , NW) = src(sizex - 1 , y , SE);
        dst(sizex - 1 , y , SW) = src(sizex - 1 , y , NE);
    }

    dst.swap(src);
}

//Check if the neighbouring cell at the 'direction' of cell (x , y) is boundary or not
bool checkBoundary (size_t x , size_t y , size_t direction , FlagField& flag)
{   bool res = false;
    if (x != 0 && x !=sizex-1 && y != 0 && y !=sizey-1 )
    {   switch (direction)
        { case W:
            res = (flag(x - 1 , y) == 0);
          case S:
            res = (flag(x , y - 1) == 0);
          case E:
            res = (flag(x + 1 , y) == 0);
          case N:
            res = (flag(x , y + 1) == 0);
          case SW:
            res = (flag(x - 1 , y - 1) == 0);
          case SE:
            res = (flag(x + 1 , y - 1) == 0);
          case NW:
            res = (flag(x - 1 , y + 1) == 0);
          case NE:
            res = (flag(x + 1 , y + 1) == 0);
        }
    }

    if (x == 0)
    {   if (direction == NW || direction == W || direction == SW)
        {   res = true;
        }
    } else if (x == sizex - 1)
    {   if (direction == NE || direction == E || direction == SE)
        {   res = true;
        }
    }

    if (y == 0)
    {   if (direction == SW || direction == S || direction == SE)
        {   res = true;
        }
    } else if (y == sizey - 1)
    {   if (direction == NW || direction == N || direction == NE)
        {   res = true;
        }
    }

    return res;
}

//Stream for Bonus task : check each neighbouring cell before stream
void streamBonus (PDF_Field& src , PDF_Field& dst , FlagField& flag)
{   for (size_t x = 0 ; x < sizex ; ++x)
    {   for (size_t y = 0 ; y < sizey ; ++y)
        {   dst(x , y , C) = src(x , y , C);

            if (checkBoundary(x , y , W , flag))
            {   dst(x , y , E) = src(x , y , W);
            } else
            {   dst(x - 1, y , W) = src(x , y , W);
            }

            if (checkBoundary(x , y , S , flag))
            {   dst(x , y , N) = src(x , y , S);
            } else
            {   dst(x , y - 1 , S) = src(x , y , S);
            }

            if (checkBoundary(x , y , E , flag))
            {   dst(x , y , W) = src(x , y , E);
            } else
            {   dst(x + 1, y , E) = src(x , y , E);
            }

            if (checkBoundary(x , y , N , flag))
            {   dst(x , y , S) = src(x , y , N);
            } else
            {   dst(x , y + 1 , N) = src(x , y , N);
            }

            if (checkBoundary(x , y , SW , flag))
            {   dst(x , y , NE) = src(x , y , SW);
            } else
            {   dst(x - 1 , y - 1 , SW) = src(x , y , SW);
            }

            if (checkBoundary(x , y , NW , flag))
            {   if (y == sizey - 1)
                {   dst(x , y , SE) = src(x , y , NW) + 0.04 / 3.0;
                } else
                {   dst(x , y , SE) = src(x , y , NW);
                }
            } else
            {   dst(x - 1 , y + 1 , NW) = src(x , y , NW);
            }

            if (checkBoundary(x , y , SE , flag))
            {   dst(x , y , NW) = src(x , y , SE);
            } else
            {   dst(x + 1 , y - 1 , SE) = src(x , y , SE);
            }

            if (checkBoundary(x , y , NE , flag))
            {   if (y == sizey - 1)
                {   dst(x , y , SW) = src(x , y , NE) - 0.04 / 3.0;
                } else
                {   dst(x , y , SW) = src(x , y , NE);
                }
            } else
            {   dst(x + 1 , y + 1 , NE) = src(x , y , NE);
            }
        }
    }

    dst.swap (src);
}

//Compute the velocity and density for every cell
void compute_val (PDF_Field& pdf , VelocityField& velocity , DensityField& density)
{   double sum = 0;
    for (size_t x = 0 ; x < sizex ; ++x)
    {   for (size_t y = 0 ; y < sizey ; ++y)
        {   for (size_t f = 0 ; f < 9 ; ++f)
            {   sum += pdf (x , y , f);
            }
            density(x , y) = sum;
            sum = 0;

            velocity(x , y , 0) = pdf(x , y , E) + pdf(x , y , SE) + pdf(x , y , NE) - 
                                  pdf(x , y , W) - pdf(x , y , SW) - pdf(x , y , NW);
            if (fabs(velocity(x , y , 0)) < 1e-15)
            {   velocity (x , y , 0) = 0;
            }

            velocity(x , y , 1) = pdf(x , y , N) + pdf(x , y , NW) + pdf(x , y , NE) - 
                                  pdf(x , y , S) - pdf(x , y , SW) - pdf(x , y , SE);
            if (fabs(velocity(x , y , 1)) < 1e-15)
            {   velocity (x , y , 1) = 0;
            }

            velocity(x , y , 2) = velocity(x , y , 0) * velocity(x , y , 0) +
                                  velocity(x , y , 1) * velocity(x , y , 1);
        }
    }
}

//Write the result to vtk file
void output_vtk (std::string vtk_file , FlagField& flag , DensityField& density , VelocityField& velocity)
{   //Check if the given directory already exist
    //Create the directory for output if it doesn't exist
    char * fileLoc = new char [vtk_file.length()+1];
    std::strcpy (fileLoc , vtk_file.c_str());
        
    char *t = strrchr(fileLoc, '/');
    *t = 0;
    if (access(fileLoc , 0) == -1)
    {   std::cout << "creat" << std::endl;
        *t = '/';
        t = fileLoc;

        while ((t = strchr(++t, '/'))) {
            *t = 0;
            if (access(fileLoc, 0) == -1)
            {   mkdir(fileLoc , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            }

            *t = '/';
        }
    }

    t = NULL;
    delete[] fileLoc;

    //Write        
    std::string filename = vtk_file.substr (0 , vtk_file.length() - 4);
    std::stringstream sstr;
    sstr << cur_timestep;
    filename += sstr.str();
    filename += ".vtk";

    std::ofstream vtk_out(filename.c_str());

    if(!vtk_out.is_open()){
        std::cerr << "Error: Failed to open \"" << filename << "\" !" << std::endl;
    } else
    {   vtk_out << "# vtk DataFile Version 4.0\n";
        vtk_out << "SiwiRVisFile\n";
        vtk_out << "ASCII\n";
        vtk_out << "DATASET STRUCTURED_POINTS\n";
        vtk_out << "DIMENSIONS " << sizex << " " << sizey << " 1\n";
        vtk_out << "ORIGIN 0 0 0\n";
        vtk_out << "SPACING 1 1 1\n";
        vtk_out << "POINT_DATA " << point_data << "\n";
        vtk_out << std::endl;

        vtk_out << "SCALARS flags double 1\n";
        vtk_out << "LOOKUP_TABLE default\n";
        for (size_t y = 0 ; y < sizey ; ++y)
        {   for (size_t x = 0 ; x < sizex ; ++x)
            {   vtk_out << flag(x , y) << std::endl;
            }
        }
        vtk_out << std::endl;

        vtk_out << "SCALARS density double 1\n";
        vtk_out << "LOOKUP_TABLE default\n";
        for (size_t y = 0 ; y < sizey ; ++y)
        {   for (size_t x = 0 ; x < sizex ; ++x)
            {   vtk_out << density(x , y) << std::endl;
            }
        }
        vtk_out << std::endl;

        vtk_out << "SCALARS velocity double\n";
        for (size_t y = 0 ; y < sizey ; ++y)
        {   for (size_t x = 0 ; x < sizex ; ++x)
            {   vtk_out << velocity(x , y , 0) << " "
                        << velocity(x , y , 1) << " "
                        << "0\n";
            }
        }
        vtk_out << std::endl;

        vtk_out.close();
    }
}

//Collision
void collision (PDF_Field& pdf , VelocityField& velocity , DensityField& density)
{   double equil = 0;   //equilibrium distribution function
    double proc = 0;    //the vector product of the discrete velocity and the fluid velocity

    for (size_t x = 0 ; x < sizex ; ++x)
    {   for (size_t y = 0 ; y < sizey ; ++y)
        {   for (size_t f = 0 ; f < 9 ; ++f)
            {   switch (f)
                {case 1:
                 case 5:
                 case 6:
                    proc -= velocity (x , y , 0);
                    break;
                 case 3:
                 case 8:
                 case 7:
                    proc += velocity (x , y , 0);
                    break;
                }
                switch (f)
                {case 2:
                 case 5:
                 case 7:
                    proc -= velocity (x , y , 1);
                    break;
                 case 4:
                 case 6:
                 case 8:
                    proc += velocity (x , y , 1);
                    break;
                }

                equil = t[f] * (density(x , y) + 3.0 * proc * (1.5 * proc + 1.0) - 1.5 * velocity (x , y , 2));
                pdf(x , y , f) = (1.0 - omega) * pdf(x , y , f) + omega * equil;
                proc = 0;
            }
        }
    }
}

int main (int argc, char* argv[]){
    if(argc != 2 && argc != 3){
        std::cerr << "correct usage is ./lbm filename [bonus]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string inputFilename (argv[1]);

    bool bonus = false;
    if (argc == 3 && strcmp(argv[2] , "bonus") == 0)
    {   bonus = true;
        std::cout << "Bonus mode activated!" << std::endl;
    } else
    {   std::cout << "Nomal mode activated!" << std::endl;
    }

    //Input parameters from configure file
    FileReader reader (inputFilename);

    if (!bonus)
    {   sizex = reader.getParameter<size_t> ("sizex");
        sizey = reader.getParameter<size_t> ("sizey");
        if (sizex == 0 || sizey == 0)
        {   std::cerr << "Invalid grid size!" << std::endl;
            return EXIT_FAILURE;
        }
    } else
    {   std::string pgmFileName = reader.getParameter<std::string> ("geometry");
        if (pgmFileName.empty() || pgmFileName.substr (pgmFileName.length() - 4) != ".pgm")
        {   std::cerr << "Invalid 'geometry' parameter!" << std::endl;
            return EXIT_FAILURE;
        }

        std::ifstream geometry (pgmFileName.c_str());

        if (!geometry.is_open())
        {   std::cerr<< "Fail to open file \"" << pgmFileName << "\" !" << std::endl;
            return EXIT_FAILURE;
        }

        char* buffer =  new char[256];
        size_t state = 0;
        std::string temp;
        std::stringstream sstr;

        while (!geometry.eof() && state <= 3) {
            geometry.getline (buffer , 100);

            if (buffer[0] != '#')
            {   ++state;

                switch (state)
                { case 0:   //Start of file
                  case 1:   //Magic Number
                    break;
                  case 2:   //Size
                    temp = strtok (buffer , " ");
                    sstr << temp;
                    sstr >> sizex;
                    sstr.clear();

                    temp = strtok (NULL , " ");
                    sstr << temp;
                    sstr >> sizey;
                    sstr.clear();
                    break;
                 case 3:    //Maximum gray value of pixels
                    if (atoi(buffer) != 255)
                    {   std::cerr << "Inappropriate geometry file!" << std::endl;
                        return EXIT_FAILURE;
                    }
                    break;
                 default:
                    break;
                }
            }
	}

        delete[] buffer;
        geometry.close();
    }

    point_data = sizex * sizey;

    const size_t timesteps = reader.getParameter<size_t> ("timesteps");
    if (timesteps == 0 || timesteps > 1000000)
    {   std::cerr << "Invalid 'timesteps' parameter!" << std::endl;
        return EXIT_FAILURE;
    }

    const size_t vtk_step = reader.getParameter<size_t> ("vtk_step");
    if (vtk_step > timesteps)
    {   std::cerr << "Invalid 'vtk_step' parameter!" << std::endl;
        return EXIT_FAILURE;
    }

    bool visual = true;
    if (vtk_step == 0)
    {   std::cout << "Visualization turned off." << std::endl;
        visual = false;
    }

    omega = reader.getParameter<double> ("omega");
    if (omega < 0.5 || omega > 1.95)
    {   std::cerr << "Invalid 'omega' parameter!\n" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string vtk_file = reader.getParameter<std::string> ("vtk_file");
    if (vtk_file.empty() || vtk_file.substr (vtk_file.length() - 4) != ".vtk")
    {   std::cerr << "Invalid 'vtk_file' parameter!\n";
        return EXIT_FAILURE;
    }

    //Create and intialize grids etc.
    PDF_Field src (sizex , sizey);
    PDF_Field dst (sizex , sizey);
    VelocityField velocity (sizex , sizey);
    DensityField density (sizex , sizey);
    FlagField flag (sizex , sizey);

    for (size_t f = 0 ; f < 9 ; ++f)
    {   switch (f)
        { case 0:
            t[f] = 4.0 / 9.0;
            break;
          case 1:
          case 2:
          case 3:
          case 4:
            t[f] = 1.0 / 9.0;
            break;
          case 5:
          case 6:
          case 7:
          case 8:
            t[f] = 1.0 / 36.0;
            break;
        }
    }
    
    for (size_t x = 0 ; x < sizex; ++x)
    {   for (size_t y = 0 ; y < sizey; ++y)
        {   for (size_t f = 0 ; f < 9 ; ++f)
            {   src (x , y , f) = t[f];
            }
            density (x , y) = 1;
        }
    }

    if (!bonus)
    {   for (size_t x = 0 ; x < sizex ; ++x)
        {   for (size_t y = 0 ; y < sizey ; ++y)
            {   flag(x , y) = 1;
            }
        }
    } else
    {   std::string pgmFileName = reader.getParameter<std::string> ("geometry");
        std::ifstream geometry (pgmFileName.c_str());

        if (!geometry.is_open())
        {   std::cerr<< "Fail to open file \"" << pgmFileName << "\" !" << std::endl;
            return EXIT_FAILURE;
        }

        char* buffer =  new char[256];
        size_t state = 0;
        size_t grayVal = 0;
        std::string temp;
        std::stringstream sstr;

        while (!geometry.eof() && state < sizey + 3) {
            geometry.getline (buffer , 256);

            if (buffer[0] != '#')
            {   ++state;

		if (state > 3)
                {   for (size_t x = 0 ; x < sizex ; ++x)
                    {   std::cout << x << std::endl;
                        if (x == 0)
                        {   temp = strtok (buffer , " ");
                        } else
                        {   temp = strtok (NULL , " ");
                        }
                        sstr << temp;
                        sstr >> grayVal;
                        sstr.clear();

                        flag(x , sizey + 3 - state) = (grayVal == 255 ? 1 : 0);
                    }
                }
            }
	}

        delete[] buffer;
        geometry.close();
    }

    //Simulation
    std::cout << "Start the simulation for a " << sizex << " * " << sizey << " grid..."<< std::endl;
    struct timeval tim;
    gettimeofday(&tim , NULL);
    double t_start = tim.tv_sec * 1000.0 + (tim.tv_usec / 1000.0);
 
    if (!bonus)
    {   do
        {   stream (src , dst);
            compute_val (src , velocity , density);

            if (visual && cur_timestep % vtk_step == 0)
            {   output_vtk (vtk_file , flag , density , velocity);
            }
        
            collision (src , velocity , density);
            ++cur_timestep;
        } while (cur_timestep <= timesteps);
    } else
    {   do
        {   streamBonus (src , dst , flag);
            compute_val (src , velocity , density);

            if (visual && cur_timestep % vtk_step == 0)
            {   output_vtk (vtk_file , flag , density , velocity);
            }

            collision (src , velocity , density);
            ++cur_timestep;
        } while (cur_timestep <= timesteps);

    }

    gettimeofday(&tim , NULL);
    double t_end = tim.tv_sec * 1000.0 + (tim.tv_usec / 1000.0);
    std::cout << "Simulating completed!" << std::endl;
    std::cout << "Time used: " << t_end - t_start << " ms." << std::endl;

    if (!visual)
    {   double MLUps = sizex * sizey * timesteps / (t_end - t_start) * 1000;
        std::cout << "MLUps :" << MLUps << std::endl;
    }

    return 0;
}
