int x0 = triangleModel[j].vertex[a][0];
                    int x1 = triangleModel[j].vertex[b][0];
                    int y0 = triangleModel[j].vertex[a][1];
                    int y1 = triangleModel[j].vertex[b][1];
                    double m = double(y1-y0)/double(x1-x0);
                    int d;
                    int yMin = min(y0, y1);
                    int yMax = max(y0, y1);
                    int xMin = min(x0, x1);
                    int xMax = max(x0, x1);
                    /*int absX = abs(x1-x0);
                    int absY = abs(y1-y0);
                    double m = double(y1-y0)/(x1-x0);
                    int d = 2*absY - absX;*/
                    if(m == INFINITY || m == -INFINITY)
                    {
                        for(y = yMin; y<yMax; y++)
                        {
                            image[x0][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x0][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x0][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                        }
                    }
                    else if(m == 0)
                    {
                        for(x = xMin; x<xMax; x++)
                        {
                            image[x][y0].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            image[x][y0].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            image[x][y0].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                        }
                    }
                    if(m <= 1.0 && m > -1.0)
                    {
                        if((x1-x0) >= 0 && (y1-y0) >= 0)
                        {
                            d = 2*(y0-y1) + (x1-x0);
                            y = y0;
                            for(x = x0; x <= x1; x++)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                if(d<0)
                                {
                                    y += 1;
                                    d += 2*((y0-y1) + 2*(x1-x0));
                                }
                                else
                                {
                                    d += 2*(y0-y1);
                                }
                            }
                            continue;
                        }
                        else if((x1-x0) <= 0 && (y1-y0) <= 0)
                        {
                            d = (-2*(y0-y1)) + (-1*(x1-x0));
                            y = y0; //maybe change
                            for(x = x0; x >= x1; x--)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                if(d<0)
                                {
                                    y -= 1;
                                    d += (-2*((y0-y1)) + (-2*(x1-x0)));
                                }
                                else
                                {
                                    d += -2*(y0-y1);
                                }
                            }
                            continue;
                        }
                    }
                    else if(m > 1.0)
                    {
                        if((x1-x0) >= 0 && (y1-y0) >= 0)
                        {
                            x = x0;
                            d = (1*(y0-y1)) + (2*(x1-x0));
                            for(y = y0; y <= y1; y++)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                if(d>0)
                                {
                                    x += 1;
                                    d += 2*((y0-y1) + 2*(x1-x0));
                                }
                                else
                                {
                                    d += 2*(x1-x0);
                                }
                            }
                            continue;
                        }
                        else if((x1-x0) <= 0 && (y1-y0) <= 0)
                        {
                            x = x0;
                            d = (-1*(y0-y1)) + (-2*(x1-x0));
                            for(y = y0; y >= y1; y--)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                if(d>0)
                                {
                                    x -= 1;
                                    d += (-2*((y0-y1)) + (-2*(x1-x0)));
                                }
                                else
                                {
                                    d += -2*(x1-x0);
                                }
                            }
                            continue;
                        }
                    }
                    else if(m < -1.0)
                    {
                        if((x1-x0) >= 0 && (y1-y0) <= 0)
                        {
                            x = x0;
                            d = (y0-y1) + (-2*(x1-x0));
                            for(y=y0; y>=y1; y--)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                if(d<0)
                                {
                                    x++;
                                    d += 2*(y0-y1) + (-2*(x1-x0));
                                }
                                else
                                {
                                    d += (-2*(x1-x0));
                                }
                            }
                            continue;
                        }
                        else if((x1-x0) <= 0 && (y1-y0) >= 0)
                        {
                            x = x0;
                            d = (-1*(y0-y1)) + 2*(x1-x0);
                            for(y=y0; y<=y1; y++)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                                if(d<0)
                                {
                                    x -= 1;
                                    d += (-2*(y0-y1)) + 2*(x1-x0);
                                }
                                else
                                {
                                    d += 2*(x1-x0);
                                }
                            }
                        }
                    }
                    else if(m >= -1.0 && m < 1.0)
                    {
                        if((x1-x0) <= 0 && (y1-y0) >= 0)
                        {
                            y = y0;
                            d = (-2*(y0-y1)) + (x1-x0);
                            for(x = x0; x >= x1; x--)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                if(d>0)
                                {
                                    y++;
                                    d += (-2*(y0-y1)) + 2*(x1-x0);
                                }
                                else
                                {
                                    d += -2*(y0-y1);
                                }
                            }
                            continue;
                        }
                        else if((x1-x0) >= 0 && (y1-y0) <= 0)
                        {
                            y = y0;
                            d = (2*(y0-y1)) + (-1*(x1-x0));
                            for(x = x0; x <= x1; x++)
                            {
                                image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                if(d>0)
                                {
                                    y--;
                                    d += (2*(y0-y1)) + (-2*(x1-x0));
                                }
                                else
                                {
                                    d += 2*(y0-y1);
                                }
                            }
                            continue;
                        }
                    }