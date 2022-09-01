using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Globalization;
using System.IO;
using System.Linq;

namespace ConsoleApp3
{
    class Program
    {
        static void Main(string[] args)
        {
            bool superRedEnabled = true;
            String fileExt = "_converted_super_red";
            while (true)
            {
                Console.WriteLine("File name:");
                String image = Console.ReadLine();
                if (image == "sro")
                {
                    superRedEnabled = !superRedEnabled;
                    Console.WriteLine($"Super-Red: {superRedEnabled}");

                    if (superRedEnabled)
                    {
                        fileExt = "_converted_super_red";

                    }
                    else
                    {
                        fileExt = "_converted_normal";
                    }

                    continue;
                }

                try
                {

                    string ImageData = System.IO.File.ReadAllText(image);

                    List<string> dims = ImageData.Split("D")[0].Split("=").ToList();
                    List<string> pixels = ImageData.Split("D")[1].Split("-").ToList();

                    Bitmap bity = new Bitmap(int.Parse(dims[0]), int.Parse(dims[1]));

                    int counter = 0;
                    int colCounter = 0;
                    Console.WriteLine($"{dims[0]} x {dims[1]}");

                    for (int i = 0; i < int.Parse(dims[1]); i++)
                    {
                        for (int j = 0; j < int.Parse(dims[0]); j++)
                        {
                            List<string> rgba = pixels[counter].Split(" ").ToList();
                            if (int.Parse(rgba[0]) >= 200 && superRedEnabled)
                            {
                                rgba[0] = "255";
                                rgba[1] = "0";
                                rgba[2] = "0";
                            }


                            if (int.Parse(rgba[3]) < 255)
                            {
                                Console.WriteLine("LOW ALPHA");
                                rgba[0] = "255";
                                rgba[1] = "255";
                                rgba[2] = "0";
                                rgba[3] = "255";
                            }

                            Color pixelColor = Color.FromArgb(int.Parse(rgba[3]), int.Parse(rgba[0]), int.Parse(rgba[1]), int.Parse(rgba[2]));
                            bity.SetPixel(j, i, pixelColor);
                            counter++;
                        }
                    }

                    Console.WriteLine($"{counter}");
                    bity.Save($"{image}{fileExt}.png", ImageFormat.Png);
                }
                catch (FileNotFoundException e)
                {
                    Console.WriteLine("Failed to find file.");
                }

            }

        }
    }
}
