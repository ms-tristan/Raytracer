// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** IObserver
*/

#ifndef SRC_IOBSERVER_IOBSERVER_HPP_
#define SRC_IOBSERVER_IOBSERVER_HPP_

namespace RayTracer {
class IObserver {
 public:
    virtual ~IObserver() = default;
    virtual void update() = 0;
};
}  // namespace RayTracer

#endif  // SRC_IOBSERVER_IOBSERVER_HPP_
