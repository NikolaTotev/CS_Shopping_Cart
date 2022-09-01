using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;

namespace Rgba_File_Visualizer
{
    class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                Console.WriteLine("Directory to process:");

                String directory = Console.ReadLine();

                if (Directory.Exists(directory))
                {
                    string[] files = Directory.GetFiles(directory);

                    foreach (string file in files)
                    {
                        try
                        {

                            string ImageData = System.IO.File.ReadAllText(file);

                            List<string> dims = ImageData.Split("D")[0].Split("=").ToList();
                            List<string> pixels = ImageData.Split("D")[1].Split("-").ToList();

                            Bitmap normalBitmap = new Bitmap(int.Parse(dims[0]), int.Parse(dims[1]));
                            Bitmap superRedBitmap = new Bitmap(int.Parse(dims[0]), int.Parse(dims[1]));

                            int counter = 0;
                            int colCounter = 0;
                            Console.WriteLine($"{dims[0]} x {dims[1]}");

                            for (int i = 0; i < int.Parse(dims[1]); i++)
                            {
                                for (int j = 0; j < int.Parse(dims[0]); j++)
                                {
                                    List<string> rgba = pixels[counter].Split(" ").ToList();

                                    Color pixelColor = Color.FromArgb(int.Parse(rgba[3]), int.Parse(rgba[0]),
                                        int.Parse(rgba[1]), int.Parse(rgba[2]));
                                    normalBitmap.SetPixel(j, i, pixelColor);

                                    if (int.Parse(rgba[0]) >= 200)
                                    {
                                        rgba[0] = "255";
                                        rgba[1] = "0";
                                        rgba[2] = "0";

                                    }


                                    if (int.Parse(rgba[3]) < 255)
                                    {
                                        Console.WriteLine("Low Alpha Detected");
                                        rgba[0] = "255";
                                        rgba[1] = "255";
                                        rgba[2] = "0";
                                        rgba[3] = "255";
                                    }

                                    pixelColor = Color.FromArgb(int.Parse(rgba[3]), int.Parse(rgba[0]),
                                        int.Parse(rgba[1]), int.Parse(rgba[2]));
                                    superRedBitmap.SetPixel(j, i, pixelColor);

                                    counter++;
                                }
                            }

                            Console.WriteLine($"{counter}");
                            normalBitmap.Save($"{file}_converted.png", ImageFormat.Png);
                            superRedBitmap.Save($"{file}_converted_super_red.png", ImageFormat.Png);
                        }
                        catch (FileNotFoundException e)
                        {
                            Console.WriteLine("Failed to find file.");
                        }
                    }
                }
                else
                {
                    Console.WriteLine("Directory does not exist!");
                }
            }
        }
    }
}

