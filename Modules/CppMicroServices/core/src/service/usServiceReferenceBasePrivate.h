/*============================================================================

  Library: CppMicroServices

  Copyright (c) German Cancer Research Center (DKFZ)
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

============================================================================*/


#ifndef USSERVICEREFERENCEBASEPRIVATE_H
#define USSERVICEREFERENCEBASEPRIVATE_H

#include "usAtomicInt_p.h"

#include "usServiceInterface.h"

#include <string>

US_BEGIN_NAMESPACE

class Any;
class Module;
class ServicePropertiesImpl;
class ServiceRegistrationBasePrivate;
class ServiceReferenceBasePrivate;


/**
 * \ingroup MicroServices
 */
class ServiceReferenceBasePrivate
{
public:

  ServiceReferenceBasePrivate(ServiceRegistrationBasePrivate* reg);

  ~ServiceReferenceBasePrivate();

  /**
    * Get the service object.
    *
    * @param module requester of service.
    * @return Service requested or null in case of failure.
    */
  void* GetService(Module* module);

  InterfaceMap GetServiceInterfaceMap(Module* module);

  /**
    * Get new service instance.
    *
    * @param module requester of service.
    * @return Service requested or null in case of failure.
    */
  InterfaceMap GetPrototypeService(Module* module);

  /**
   * Unget the service object.
   *
   * @param module Module who wants remove service.
   * @param checkRefCounter If true decrement reference counter and remove service
   *                        if we reach zero. If false remove service without
   *                        checking reference counter.
   * @return True if service was removed or false if only reference counter was
   *         decremented.
   */
  bool UngetService(Module* module, bool checkRefCounter);

  /**
   * Unget prototype scope service objects.
   *
   * @param module Module who wants to remove a prototype scope service.
   * @param service The prototype scope service pointer.
   * @return \c true if the service was removed, \c false otherwise.
   */
  bool UngetPrototypeService(Module* module, void* service);

  bool UngetPrototypeService(Module* module, const InterfaceMap& service);

  /**
   * Get all properties registered with this service.
   *
   * @return A ServiceProperties object containing properties or being empty
   *         if service has been removed.
   */
  const ServicePropertiesImpl& GetProperties() const;

  /**
   * Returns the property value to which the specified property key is mapped
   * in the properties <code>ServiceProperties</code> object of the service
   * referenced by this <code>ServiceReference</code> object.
   *
   * <p>
   * Property keys are case-insensitive.
   *
   * <p>
   * This method must continue to return property values after the service has
   * been unregistered. This is so references to unregistered services can
   * still be interrogated.
   *
   * @param key The property key.
   * @param lock If <code>true</code>, access of the properties of the service
   * referenced by this <code>ServiceReference</code> object will be
   * synchronized.
   * @return The property value to which the key is mapped; an invalid Any
   * if there is no property named after the key.
   */
  Any GetProperty(const std::string& key, bool lock) const;

  bool IsConvertibleTo(const std::string& interfaceId) const;

  /**
   * Reference count for implicitly shared private implementation.
   */
  AtomicInt ref;

  /**
   * Link to registration object for this reference.
   */
  ServiceRegistrationBasePrivate* const registration;

  /**
   * The service interface id for this reference.
   */
  std::string interfaceId;

private:

  InterfaceMap GetServiceFromFactory(Module* module, ServiceFactory* factory,
                                     bool isModuleScope);

  // purposely not implemented
  ServiceReferenceBasePrivate(const ServiceReferenceBasePrivate&);
  ServiceReferenceBasePrivate& operator=(const ServiceReferenceBasePrivate&);
};

US_END_NAMESPACE

#endif // USSERVICEREFERENCEBASEPRIVATE_H
